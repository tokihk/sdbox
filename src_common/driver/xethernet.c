/*
 * xethernet.c
 *
 *  Created on: 2024/03/27
 *      Author: Hitoshi Kono
 */

#include "xethernet.h"


/* ----------------------------------------------
 * Macros for defining device lists
 * ------------------------------------------- */
struct xether_device_info
{
	bool_t						(*api_open)(const xether_config_t *config);
	void						(*api_close)(void);
	struct pbuf *				(*api_recv_packet_get)(void);
	bool_t						(*api_send_packet_set)(struct pbuf *p);
	bool_t						(*api_link_status_update)(void);
};

#define XETHERNET_DEVICE_LIST_BEGIN()		static const struct xether_device_info XETHERNET_DEVICE_LIST[] = {
#define XETHERNET_DEVICE_LIST_END()			};
#define XETHERNET_DEVICE_LIST_ITEM(prefix)	\
{															\
	.api_open=prefix##_open,								\
	.api_close=prefix##_close,								\
	.api_recv_packet_get=prefix##_recv_packet_get, 			\
	.api_send_packet_set=prefix##_send_packet_set,			\
	.api_link_status_update=prefix##_link_status_update,	\
}

/* ----------------------------------------------
 * Import board module
 * ------------------------------------------- */
#if XBOARD_ETHERNET_ENABLE
  #include <driver/xethernet_board.h>

  #define XBOARD_ETHERNET_NUM	(XCOUNTOF(XETHERNET_DEVICE_LIST))

#else
  #define XBOARD_ETHERNET_NUM				(0)

#endif


typedef struct xether_object
{
	uint16_t					dev_id;

	xether_config_t				config;

	uint32_t					tick_link_last;

	uint32_t					rx_packet_count;
	uint32_t					tx_packet_count;

	bool_t						is_opened;
	bool_t						is_linkup;
} xether_object_t;

static struct
{
	xether_object_t				objs[XBOARD_ETHERNET_NUM];

	bool_t						is_init;
} g_xether;


/* ------------------------------------------------------------------ */
/* TX（ゼロコピー）                                                     */
/* ------------------------------------------------------------------ */

/**
 * @brief netif出力関数（lwIPから呼ばれる送信エントリポイント）
 *
 * pbuf チェーンをたどって各セグメントをTXキューに積む。
 * ゼロコピー: pbuf->payload をそのままBDにセットする。
 * DMA完了まで pbuf を保護するため pbuf_ref() する。
 */
static err_t ethernetif_linkoutput(struct netif *netif, struct pbuf *p)
{
    ethernetif_data_t *data = (ethernetif_data_t *)netif->state;
    netc_hal_ctx_t    *ctx  = data->hal_ctx;

    LINK_STATS_INC(link.xmit);

    /*
     * lwIPはチェーンpbufを渡してくる場合がある。
     * NETCはSGDMA（Scatter-Gather）をサポートするが、
     * シンプル化のため先頭セグメントのみゼロコピー送信し、
     * チェーンの場合は1つのフラットバッファにコピーする方式も選択肢。
     *
     * ここでは、チェーンpbufを1件ずつゼロコピーで送信する設計とする。
     * （NETCのSG対応は NETC_EnetcSendFrame の flags で設定可能）
     */
    for (struct pbuf *q = p; q != NULL; q = q->next) {
        /* DMA完了まで pbuf を解放させないよう参照カウントを増やす */
        pbuf_ref(q);

        netc_hal_tx_frame_t tx_frame = {
            .data   = (const uint8_t *)q->payload,
            .length = (uint16_t)q->len,
            .pbuf   = q,   /* reclaim時に pbuf_free() するために保持 */
        };

        int ret = netc_hal_tx_send(ctx, &tx_frame);
        if (ret != 0) {
            /* リング満杯: 今回は ref を戻して ERR_IF を返す
             * 呼び出し元 lwIP が再送を試みる                    */
            pbuf_free(q);
            LINK_STATS_INC(link.drop);
            return ERR_IF;
        }
    }

    return ERR_OK;
}

/* ------------------------------------------------------------------ */
/* TX reclaim（pbuf解放）                                              */
/* ------------------------------------------------------------------ */

/**
 * @brief TX完了済みpbufを解放する
 *
 * ethernetif_poll() から呼ばれる。
 * HALが回収したBDに対応するpbufを pbuf_free() する。
 *
 * 注意: HALはpbufポインタをctx->tx_pending[]に保持している。
 *       ここではHALのpendingリストを走査して解放する。
 *       HAL側はin_use=falseにしたスロットのpbufを返す。
 */
static void ethernetif_tx_reclaim(ethernetif_data_t *data)
{
    /*
     * netc_hal_tx_reclaim() を呼ぶと内部でin_use=falseになる。
     * 本来はコールバック形式（on_tx_done(pbuf)）の方が綺麗だが、
     * 今回はHALのpendingリストにアクセスするAPI経由で解放する。
     *
     * 実用上はHALにtx_done_cbを持たせてこちらで登録するか、
     * HALから「回収済みpbufリスト」を受け取るAPIにする。
     * ここではシンプルに示すため HAL 内部で pbuf_free まで行う設計例を示す。
     */
    netc_hal_tx_reclaim(data->hal_ctx);

    /*
     * --- 代替設計メモ ---
     * HALにコールバックを登録する方式:
     *
     *   void on_tx_done(void *pbuf_ptr) {
     *       pbuf_free((struct pbuf *)pbuf_ptr);
     *   }
     *
     *   netc_hal_tx_reclaim_with_cb(ctx, on_tx_done);
     *
     * この方式にするとHAL側がlwIPに依存せず済む。
     */
}

/* ------------------------------------------------------------------ */
/* RX（ゼロコピー）                                                    */
/* ------------------------------------------------------------------ */

/**
 * @brief 受信フレームをlwIPスタックに渡す
 *
 * DMAバッファをコピーせず、カスタムpbufでラップしてlwIPに渡す。
 * pbuf_free() が呼ばれた時点でHWにバッファが返却される。
 */
static void ethernetif_input(struct netif *netif,
                              ethernetif_data_t *data,
                              const netc_hal_rx_frame_t *rx_frame)
{
    /* カスタムpbufスロット確保 */
    rx_zero_copy_pbuf_t *custom = rx_custom_alloc();
    if (custom == NULL) {
        /* プール枯渇: バッファを即座にHWに返却してドロップ */
        netc_hal_rx_release(data->hal_ctx, rx_frame);
        LINK_STATS_INC(link.drop);
        MIB2_STATS_NETIF_INC(netif, ifindiscards);
        return;
    }

    /* カスタムpbuf初期化 */
    custom->hal_ctx  = data->hal_ctx;
    custom->rx_frame = *rx_frame; /* bd_idx を保持 */

    custom->pbuf_custom.custom_free_function = rx_pbuf_free_custom;

    /*
     * pbuf_alloced_custom() でDMAバッファをpbufにラップ。
     * PBUF_REF: 外部バッファを参照する（コピーなし）
     */
    struct pbuf *p = pbuf_alloced_custom(
        PBUF_RAW,
        (uint16_t)rx_frame->length,
        PBUF_REF,
        &custom->pbuf_custom,
        rx_frame->data,
        NETC_HAL_MAX_FRAME_SIZE
    );

    if (p == NULL) {
        /* アロケート失敗 */
        s_rx_custom_in_use[custom - s_rx_custom_pool] = false;
        netc_hal_rx_release(data->hal_ctx, rx_frame);
        LINK_STATS_INC(link.drop);
        MIB2_STATS_NETIF_INC(netif, ifindiscards);
        return;
    }

    MIB2_STATS_NETIF_ADD(netif, ifinoctets, p->tot_len);
    LINK_STATS_INC(link.recv);

    /* lwIPスタックに渡す（ここからスタック内部でpbufを処理） */
    err_t err = netif->input(p, netif);
    if (err != ERR_OK) {
        /* スタックが受け取れない場合はここで解放 → HWへのバッファ返却も発生 */
        pbuf_free(p);
        LINK_STATS_INC(link.drop);
    }
}

/* ------------------------------------------------------------------ */
/* 公開API実装                                                         */
/* ------------------------------------------------------------------ */

err_t ethernetif_init(struct netif *netif)
{
    assert(netif != NULL);

    /*
     * netif_add() の第4引数（state）として渡された
     * 初期化済み netc_hal_ctx_t* を受け取る。
     * HAL初期化はここでは行わない。
     */
    if (netif->state == NULL) {
        return ERR_ARG;
    }
    netc_hal_ctx_t *hal_ctx = (netc_hal_ctx_t *)netif->state;

    /* ドライバ内部コンテキストを確保 */
    static ethernetif_data_t s_drv_data; /* シングルインスタンス想定 */
    memset(&s_drv_data, 0, sizeof(s_drv_data));

    /* 初期化済みHALコンテキストをそのまま保持する */
    s_drv_data.hal_ctx = hal_ctx;

    /* netif->state をドライバデータに切り替え */
    netif->state = &s_drv_data;

    /* lwIP netif 設定 */
#if LWIP_NETIF_HOSTNAME
    netif->hostname = "rt1180";
#endif

    netif->name[0] = 'e';
    netif->name[1] = 'n';

    netif->output      = etharp_output;      /* IPv4 ARP処理 */
#if LWIP_IPV6
    netif->output_ip6  = ethip6_output;
#endif
    netif->linkoutput  = ethernetif_linkoutput; /* ゼロコピーTX */

    netif->mtu         = 1500U;
    netif->flags       = NETIF_FLAG_BROADCAST
                       | NETIF_FLAG_ETHARP
                       | NETIF_FLAG_ETHERNET;
#if LWIP_IGMP
    netif->flags      |= NETIF_FLAG_IGMP;
#endif

    /* MACアドレス設定 */
    netif->hwaddr_len = ETH_HWADDR_LEN;
    netc_hal_get_mac(s_drv_data.hal_ctx, netif->hwaddr);

    /* カスタムpbufプールの初期化 */
    memset(s_rx_custom_pool, 0, sizeof(s_rx_custom_pool));
    memset(s_rx_custom_in_use, 0, sizeof(s_rx_custom_in_use));

    /* リンク状態の確認と設定 */
    if (netc_hal_link_is_up(s_drv_data.hal_ctx)) {
        netif_set_link_up(netif);
    }

    return ERR_OK;
}

void ethernetif_poll(struct netif *netif)
{
    assert(netif != NULL);

    ethernetif_data_t *data = (ethernetif_data_t *)netif->state;

    /* TX完了済みpbufを解放（ゼロコピーTXの後処理） */
    ethernetif_tx_reclaim(data);

    /* RXポーリング：受信フレームがなくなるまでループ */
    netc_hal_rx_frame_t rx_frame;
    while (netc_hal_rx_poll(data->hal_ctx, &rx_frame)) {
        ethernetif_input(netif, data, &rx_frame);
    }

    /* リンク状態の変化を反映 */
    if (netc_hal_link_is_up(data->hal_ctx)) {
        if (!netif_is_link_up(netif)) {
            netif_set_link_up(netif);
        }
    } else {
        if (netif_is_link_up(netif)) {
            netif_set_link_down(netif);
        }
    }
}


void xether_init(void)
{
#if XBOARD_ETHERNET_ENABLE
	xether_init_board();
#endif

	g_xether.is_init = TRUE;
}

void xether_deinit(void)
{
#if XBOARD_ETHERNET_ENABLE
	xether_deinit_board();
#endif

	g_xether.is_init = FALSE;
}

uint16_t xether_device_num_get(void)
{
	return (XBOARD_ETHERNET_NUM);
}

bool_t xether_open(uint16_t id, const xether_config_t *config)
{
	bool_t open_ok = FALSE;

#if XBOARD_ETHERNET_ENABLE

	if (   (g_xether.is_init)
		&& (id < XCOUNTOF(g_xether.objs))
		&& (config != NULL)
	) {
		xether_object_t *obj = &g_xether.objs[id];

		/* ステータス初期化 */
		obj->tx_packet_count = 0;
		obj->rx_packet_count = 0;

		if (!obj->is_opened)
		{
			obj->dev_id = id;
			obj->config = *config;

			/* デバイスオープン */
			if ((XETHERNET_DEVICE_LIST[id].api_open)(config))
			{
				obj->is_opened = TRUE;

				/* リンク状態更新 */
				xether_link_status_update(obj->dev_id);
			}
		}

		open_ok = obj->is_opened;
	}
#endif

	return (open_ok);
}

void xether_close(uint16_t id)
{
#if XBOARD_ETHERNET_ENABLE
	if (id < XCOUNTOF(g_xether.objs))
	{
		xether_object_t *obj = &g_xether.objs[id];

		/* デバイスクローズ */
		if (obj->is_opened)
		{
			obj->is_opened = FALSE;

			(XETHERNET_DEVICE_LIST[id].api_close)();
		}

		/* リンク状態更新 */
		xether_link_status_update(obj->dev_id);
	}
#endif
}

bool_t xether_config_get(uint16_t id, xether_config_t *config)
{
	bool_t get_ok = FALSE;

#if XBOARD_ETHERNET_ENABLE
	if (   (id < XCOUNTOF(g_xether.objs))
		&& (config != NULL)
	) {
		xether_object_t *obj = &g_xether.objs[id];

		*config = obj->config;

		get_ok = TRUE;
	}
#endif

	return (get_ok);
}

bool_t xether_link_status_update(uint16_t id)
{
	bool_t is_link = FALSE;

#if XBOARD_ETHERNET_ENABLE
	if (id < XCOUNTOF(g_xether.objs))
	{
		xether_object_t *obj = &g_xether.objs[id];

		if (obj->is_opened)
		{
			obj->is_linkup = (XETHERNET_DEVICE_LIST[id].api_link_status_update)();
		}
		else
		{
			obj->is_linkup = FALSE;
		}

		is_link = obj->is_linkup;
	}
#endif

	return (is_link);
}

struct pbuf *xether_recv_packet_get(uint16_t id)
{
	struct pbuf *pkt = NULL;

#if XBOARD_ETHERNET_ENABLE
	if (id < XCOUNTOF(g_xether.objs))
	{
		pkt = (XETHERNET_DEVICE_LIST[id].api_recv_packet_get)();
	}
#endif

	return (pkt);
}

bool_t xether_send_packet_set(uint16_t id, struct pbuf *pkt)
{
	bool_t set_ok = FALSE;

#if XBOARD_ETHERNET_ENABLE
	if (id < XCOUNTOF(g_xether.objs))
	{
		set_ok = (XETHERNET_DEVICE_LIST[id].api_send_packet_set)(pkt);
	}
#endif

	return (set_ok);
}
