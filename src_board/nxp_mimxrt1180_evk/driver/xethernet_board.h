/*
 * xethernet_board.h
 *
 *  Created on: 2024/03/27
 *      Author: Hitoshi Kono
 */

#ifndef XETHERNET_BOARD_H_
#define XETHERNET_BOARD_H_


#include "fsl_netc_endpoint.h"      /* netc_ep_handle_t, NETC_EnetcInit()  */
#include "fsl_netc_switch.h"        /* netc_swt_handle_t, NETC_SwtInit()   */
#include "fsl_netc_mdio.h"          /* NETC_MDIOInit(), NETC_MDIOWrite()   */
#include "fsl_phyrtl8211f.h"        /* PHY_RTL8211F_Init() など            */
#include "fsl_rgpio.h"               /* GPIO_PinWrite()                     */
#include "fsl_iomuxc.h"             /* IOMUXC_SetPinMux()                  */
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/* lwIP */
#include "lwip/pbuf.h"
#include "lwip/mem.h"

/* NXP SDK ヘッダ（ターゲット環境に合わせてパスを調整） */
#include "fsl_netc_endpoint.h"      /* netc_ep_handle_t, NETC_EnetcXxx() */
#include "fsl_netc.h"            /* netc_si_config_t                    */


/* PHYのMDIOアドレス */
/* TODO: 実機のPHYストラップ(AD0/AD1ピン)設定に合わせて確認すること */
#define XETHER_EP0_PHY_ADDR					(0x01U)

/* NETC MDIOのソースクロック */
/* TODO: 実際のNETC/MDIOのソースクロック周波数に合わせて確認すること */
#define XETHER_EP0_MDIO_SRC_CLOCK_HZ			(200000000U)

/* ------------------------------------------------------------------ */
/* MIMXRT1180-EVK Ethernet構成                                         */
/*                                                                     */
/*   ETH0 : NETC SWT PORT0  RTL8211FDI-CG  RGMII  PHY addr=0x01      */
/*   ETH1 : NETC SWT PORT1  RTL8211FDI-CG  RGMII  PHY addr=0x02      */
/*   ETH2 : NETC SWT PORT2  RTL8211FDI-CG  RGMII  PHY addr=0x03      */
/*   ETH3 : NETC SWT PORT3  RTL8211FDI-CG  RGMII  PHY addr=0x04      */
/*   ETH4 : NETC EP0/SI0    RTL8211FDI-CG  RGMII  PHY addr=0x05      */
/*                                                                     */
/*   MDIO : NETC EMDIO (全PHY共有)                                    */
/*   PHY RESET: GPIO13_IO16 (アクティブLow, 全PHY共通)               */
/* ------------------------------------------------------------------ */

/** PHY RESETピン（EVK回路図より） */
#define XETHER_PHY_RESET_GPIO       GPIO13
#define XETHER_PHY_RESET_PIN        (16U)
#define XETHER_PHY_RESET_ASSERT     (0U)   /* Low = リセット中 */
#define XETHER_PHY_RESET_DEASSERT   (1U)

/** PHY RESETパルス幅 / 安定待ちウェイト（RTL8211F データシート要件） */
#define XETHER_PHY_RESET_HOLD_MS    (10U)
#define XETHER_PHY_RESET_STABLE_MS  (30U)

/** PHYアドレス（MDIO） */
#define XETHER_ETH0_PHY_ADDR    (0x01U)
#define XETHER_ETH1_PHY_ADDR    (0x02U)
#define XETHER_ETH2_PHY_ADDR    (0x03U)
#define XETHER_ETH3_PHY_ADDR    (0x04U)
#define XETHER_ETH4_PHY_ADDR    (0x05U)

/** RX BDリング深さ（2の冪、NETCハードウェア制約）*/
#define NETC_HAL_RX_BD_NUM      (32U)

/** TX BDリング深さ */
#define NETC_HAL_TX_BD_NUM      (16U)

/** Ethernetフレーム最大サイズ（VLAN対応） */
#define NETC_HAL_MAX_FRAME_SIZE (1522U)

/** RXゼロコピー用バッファ数（BDリング深さ以上必要） */
#define NETC_HAL_RX_BUF_NUM     (NETC_HAL_RX_BD_NUM)

/** バッファアライメント（NETC DMAは64バイトアライメント要求） */
#define NETC_HAL_BUF_ALIGN      (64U)


typedef struct netc_hal_ctx
{
	ep_handle_t *		ep;           /**< 外部注入済みENETCハンドル */
	uint8_t				mac_addr[6];
	uint8_t				si_idx;

	/* TX 完了追跡：ゼロコピーTXでpbufポインタを保持する */
	struct {
		void *			pbuf;     /**< 対応するpbufポインタ（NULLなら空き）*/
		bool			in_use;
	} tx_pending[NETC_HAL_TX_BD_NUM];

	uint32_t			tx_prod_idx; /**< TX生産インデックス（次にキューするBD）*/
	uint32_t			tx_cons_idx; /**< TX消費インデックス（次にreclaimするBD）*/

	uint32_t			rx_cons_idx; /**< RX消費インデックス */
} netc_hal_ctx_t;

/**
 * @brief ゼロコピーRXバッファディスクリプタ
 *
 * lwIPドライバがこの構造体でDMAバッファのオーナーシップを受け取り、
 * pbuf処理後に netc_hal_rx_release() で返却する。
 */
typedef struct
{
	uint8_t *			data;    /**< DMAバッファ先頭ポインタ（物理=仮想、RT1180はフラットメモリ）*/
	uint16_t			length;  /**< 受信フレーム長（Ethernetヘッダ〜FCS直前）*/
	uint32_t			bd_idx;  /**< BDリングインデックス（返却時に必要）*/
} netc_hal_rx_frame_t;

/**
 * @brief ゼロコピーTXバッファディスクリプタ
 *
 * lwIPドライバがpbuf->payloadポインタをそのまま渡す。
 * NETCがDMA完了後に netc_hal_tx_reclaim() でpbufを解放できる。
 */
typedef struct
{
	const uint8_t *		data;   /**< 送信データ先頭（pbuf->payload）*/
	uint16_t			length; /**< 送信バイト数 */
	void *				pbuf;   /**< 完了後に pbuf_free() するpbufポインタ */
} netc_hal_tx_frame_t;

/**
 * @brief HAL初期化パラメータ
 *
 * ペリフェラル初期化済みのハンドル・アドレスを外部から注入する。
 * NXP MCUXpresso SDK の ep_handle / si_handle を想定。
 */
typedef struct
{
	void *				ep_handle;     /**< netc_ep_handle_t* をキャストして渡す */
	uint8_t				mac_addr[6];   /**< ステーションインタフェースのMACアドレス */
	uint8_t				si_idx;        /**< ステーションインタフェース番号 (0-based) */
} netc_hal_init_params_t;


static struct
{
	bool_t						last_link_up;		/* 最新のリンクステータス */

	uint8_t						si_idx;				/**< ステーションインタフェース番号（通常0）*/
	uint32_t					phy_addr;			/**< MDIOアドレス（通常XETHER_ETH4_PHY_ADDR）*/

	/* SDK ハンドル（アプリ層が所有し、HALに注入する） */
	ep_handle_t					ep_handle;			/**< NETC ENETC EP0 ハンドル */
	netc_hal_ctx_t *			hal_ctx;			/**< HALコンテキスト（netc_hal_init後に設定）*/

	/* PHY ハンドル（RTL8211Fドライバ） */
	phy_rtl8211f_resource_t		phy_resource;		/**< MDIOアクセスリソース */
	phy_handle_t				phy_handle;			/**< PHYハンドル */

	bool_t						opened;				/**< オープン済みフラグ */

} g_xether_board;

/** シングルインスタンス（RTOSなしの場合は静的確保で十分） */
static struct netc_hal_ctx s_ctx;

/* ------------------------------------------------------------------ */
/* 内部バッファ（DMA到達可能なセクションに配置：.noncacheable or MPU設定）*/
/* ------------------------------------------------------------------ */

/*
 * RT1180 では OCRAM2 / FlexSPI SRAM が DMA 到達可能。
 * リンカスクリプトで NonCacheable セクションを定義し、
 * AT_NONCACHEABLE_SECTION マクロで配置する（MCUXpresso SDK流儀）。
 */
#ifndef AT_NONCACHEABLE_SECTION_ALIGN
/* ターゲット外（ホストテストなど）のフォールバック定義 */
#define AT_NONCACHEABLE_SECTION_ALIGN(var, align) \
    __attribute__((aligned(align))) var
#endif

/** RX DMA バッファプール */
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_rx_bufs[NETC_HAL_RX_BUF_NUM][NETC_HAL_MAX_FRAME_SIZE],
    NETC_HAL_BUF_ALIGN
);

/** TX BD リング用バッファ（SDKが要求するアライメント） */
AT_NONCACHEABLE_SECTION_ALIGN(
    static netc_tx_bd_t s_tx_bds[NETC_HAL_TX_BD_NUM],
    NETC_HAL_BUF_ALIGN
);

/** RX BD リング用バッファ */
AT_NONCACHEABLE_SECTION_ALIGN(
    static netc_rx_bd_t s_rx_bds[NETC_HAL_RX_BD_NUM],
    NETC_HAL_BUF_ALIGN
);


static inline uint32_t tx_ring_next(uint32_t idx)
{
    return (idx + 1U) % NETC_HAL_TX_BD_NUM;
}

static inline uint32_t rx_ring_next(uint32_t idx)
{
    return (idx + 1U) % NETC_HAL_RX_BD_NUM;
}

int netc_hal_init(const netc_hal_init_params_t *params, netc_hal_ctx_t **out_ctx)
{
    assert(params != NULL);
    assert(params->ep_handle != NULL);
    assert(out_ctx != NULL);

    struct netc_hal_ctx *ctx = &s_ctx;
    memset(ctx, 0, sizeof(*ctx));

    ctx->ep     = (netc_ep_handle_t *)params->ep_handle;
    ctx->si_idx = params->si_idx;
    memcpy(ctx->mac_addr, params->mac_addr, 6U);

    /* ----------------------------------------------------------------
     * BDリングをSDKに登録する
     *
     * NXP SDK では NETC_EnetcActiveSIBaseConfig() などで
     * BDリングのアドレス・深さを設定する。
     * 以下は SDK API のラッパー呼び出し例。
     * 実際の API 名・引数はSDKバージョンに合わせて変更すること。
     * ---------------------------------------------------------------- */
    netc_si_ring_config_t ring_cfg = {
        .rxRingConfig[0] = {
            .bdArray    = s_rx_bds,
            .len        = NETC_HAL_RX_BD_NUM,
            .extendLen  = 0U,
        },
        .txRingConfig[0] = {
            .bdArray    = s_tx_bds,
            .len        = NETC_HAL_TX_BD_NUM,
        },
    };

    status_t status = NETC_EnetcConfigureSIRing(ctx->ep, params->si_idx, &ring_cfg);
    if (status != kStatus_Success) {
        return -1;
    }

    /* RX BDにDMAバッファを事前に登録する（ゼロコピーRXの要） */
    for (uint32_t i = 0U; i < NETC_HAL_RX_BD_NUM; i++) {
        status = NETC_EnetcSetRxBufferAddr(ctx->ep,
                                           params->si_idx,
                                           /*ring=*/0U,
                                           /*bd_idx=*/i,
                                           s_rx_bufs[i]);
        if (status != kStatus_Success) {
            return -1;
        }
    }

    ctx->rx_cons_idx = 0U;
    ctx->tx_prod_idx = 0U;
    ctx->tx_cons_idx = 0U;

    *out_ctx = ctx;
    return 0;
}

void netc_hal_deinit(netc_hal_ctx_t *ctx)
{
    if (ctx == NULL) {
        return;
    }
    /* TX保留中のpbufをすべて解放（pbuf_free はlwIPのAPI。依存を避けるため
     * コールバック形式にしたい場合はパラメータで関数ポインタを受け取ること） */
    for (uint32_t i = 0U; i < NETC_HAL_TX_BD_NUM; i++) {
        ctx->tx_pending[i].pbuf   = NULL;
        ctx->tx_pending[i].in_use = false;
    }
}

bool netc_hal_rx_poll(netc_hal_ctx_t *ctx, netc_hal_rx_frame_t *frame)
{
    assert(ctx != NULL);
    assert(frame != NULL);

    uint16_t frame_len = 0U;

    /*
     * NETC_EnetcGetRxFrame() はBDリングをチェックし、
     * 受信済みならバッファアドレスと長さを返す。
     * 戻り値 kStatus_NoData のときは受信なし。
     *
     * ゼロコピー: データはすでに s_rx_bufs[bd_idx] にある。
     *             このポインタをそのまま呼び出し元に渡す。
     */
    netc_rx_frame_attr_t attr = {0};
    status_t st = NETC_EnetcGetRxFrame(ctx->ep,
                                        ctx->si_idx,
                                        /*ring=*/0U,
                                        ctx->rx_cons_idx,
                                        &frame_len,
                                        &attr);
    if (st == kStatus_NoData) {
        return false;
    }
    if (st != kStatus_Success) {
        /* エラーフレーム: BDを進めてスキップ */
        ctx->rx_cons_idx = rx_ring_next(ctx->rx_cons_idx);
        return false;
    }

    frame->data   = s_rx_bufs[ctx->rx_cons_idx];
    frame->length = frame_len;
    frame->bd_idx = ctx->rx_cons_idx;

    /* cons_idx を進める（返却はnetc_hal_rx_release()で行う） */
    ctx->rx_cons_idx = rx_ring_next(ctx->rx_cons_idx);

    return true;
}

void netc_hal_rx_release(netc_hal_ctx_t *ctx, const netc_hal_rx_frame_t *frame)
{
    assert(ctx != NULL);
    assert(frame != NULL);

    /*
     * BDをHWに返却する（同じバッファアドレスを再設定し、
     * BD の ownerビットをHWに渡す）。
     */
    (void)NETC_EnetcSetRxBufferAddr(ctx->ep,
                                     ctx->si_idx,
                                     /*ring=*/0U,
                                     frame->bd_idx,
                                     frame->data);

    /* HW に BD 再利用を通知するプロデューサリングをインクリメント */
    NETC_EnetcRefillRxRing(ctx->ep, ctx->si_idx, /*ring=*/0U, /*count=*/1U);
}

int netc_hal_tx_send(netc_hal_ctx_t *ctx, const netc_hal_tx_frame_t *frame)
{
    assert(ctx != NULL);
    assert(frame != NULL);
    assert(frame->data != NULL);

    /* TXリング満杯チェック */
    uint32_t next_prod = tx_ring_next(ctx->tx_prod_idx);
    if (next_prod == ctx->tx_cons_idx) {
        /* リング満杯：呼び出し元はtx_reclaim後に再試行すること */
        return -1;
    }

    /* pbufポインタを保存（TX完了後のpbuf_free用） */
    ctx->tx_pending[ctx->tx_prod_idx].pbuf   = frame->pbuf;
    ctx->tx_pending[ctx->tx_prod_idx].in_use = true;

    /*
     * BDにペイロードポインタをセットしてDMA起動。
     * ゼロコピー: pbuf->payload をそのままBDに渡す。
     *             SDKが物理アドレスへの変換を行う（RT1180はフラットマップ）。
     */
    netc_tx_frame_info_t tx_info = {
        .dataAddr = (uint32_t)(uintptr_t)frame->data,
        .dataLen  = frame->length,
        .flags    = 0U,    /* チェックサムオフロード等が必要なら設定 */
    };

    status_t st = NETC_EnetcSendFrame(ctx->ep,
                                       ctx->si_idx,
                                       /*ring=*/0U,
                                       &tx_info,
                                       ctx->tx_prod_idx);
    if (st != kStatus_Success) {
        ctx->tx_pending[ctx->tx_prod_idx].in_use = false;
        ctx->tx_pending[ctx->tx_prod_idx].pbuf   = NULL;
        return -1;
    }

    ctx->tx_prod_idx = next_prod;
    return 0;
}

void netc_hal_tx_reclaim(netc_hal_ctx_t *ctx)
{
    assert(ctx != NULL);

    /*
     * TX完了済みBDを回収する。
     * NETC_EnetcReclaimTxDescriptor() は完了したBD数を返す。
     */
    uint32_t reclaimed = 0U;
    (void)NETC_EnetcReclaimTxDescriptor(ctx->ep,
                                          ctx->si_idx,
                                          /*ring=*/0U,
                                          &reclaimed);

    for (uint32_t i = 0U; i < reclaimed; i++) {
        if (ctx->tx_pending[ctx->tx_cons_idx].in_use) {
            /* pbufはethernetif.cでlwIP APIを使って解放する。
             * ここではNULLクリアのみ。実際のpbuf_freeは
             * ethernetif_tx_reclaim() 経由で呼ぶ設計とする。 */
            ctx->tx_pending[ctx->tx_cons_idx].in_use = false;
            /* pbuf ポインタは呼び出し元 (ethernetif) が読み取って解放する */
        }
        ctx->tx_cons_idx = tx_ring_next(ctx->tx_cons_idx);
    }
}

bool netc_hal_link_is_up(netc_hal_ctx_t *ctx)
{
    assert(ctx != NULL);

    netc_port_phy_status_t phy_status = {0};
    status_t st = NETC_EnetcGetPortPhyStatus(ctx->ep, ctx->si_idx, &phy_status);
    if (st != kStatus_Success) {
        return false;
    }
    return (bool)phy_status.linkUp;
}

void netc_hal_get_mac(netc_hal_ctx_t *ctx, uint8_t mac[6])
{
    assert(ctx != NULL);
    assert(mac != NULL);
    memcpy(mac, ctx->mac_addr, 6U);
}
/* ------------------------------------------------------------------ */
/* ゼロコピーRX用カスタムpbuf                                         */
/* ------------------------------------------------------------------ */

/**
 * @brief ゼロコピーRX用カスタムpbuf拡張構造体
 *
 * pbuf_alloced_custom() でDMAバッファをラップする際に使用する。
 * pbuf の参照カウントが 0 になると lwIP が custom_free_function を呼び出し、
 * その中でDMAバッファをHWに返却する。
 *
 * 必ず先頭メンバを struct pbuf_custom にすること（lwIP要件）。
 */
typedef struct {
    struct pbuf_custom  pbuf_custom;  /**< 先頭固定（lwIP要件）           */
    netc_hal_rx_frame_t rx_frame;     /**< HW返却に必要なBD情報           */
} xether_rx_zc_pbuf_t;

/** カスタムpbufプール（BDリング深さと1対1に対応させる） */
static xether_rx_zc_pbuf_t s_rx_zc_pool[NETC_HAL_RX_BUF_NUM];
static bool                s_rx_zc_in_use[NETC_HAL_RX_BUF_NUM];

/**
 * @brief カスタムpbuf解放コールバック（lwIPから呼ばれる）
 *
 * pbuf の参照カウントが 0 になったとき lwIP が呼び出す。
 * DMAバッファをHWに返却し、プールスロットを空きに戻す。
 */
static void xether_rx_pbuf_free_custom(struct pbuf *p)
{
    xether_rx_zc_pbuf_t *slot = (xether_rx_zc_pbuf_t *)p;

    /* DMAバッファをHWのBDリングに返却する */
    netc_hal_rx_release(g_xether_board.hal_ctx, &slot->rx_frame);

    /* プールスロットを空きに戻す */
    for (uint32_t i = 0U; i < NETC_HAL_RX_BUF_NUM; i++) {
        if (&s_rx_zc_pool[i] == slot) {
            s_rx_zc_in_use[i] = false;
            break;
        }
    }
}

/**
 * @brief カスタムpbufプールからスロットを1件確保する
 * @return 確保したスロット、プール枯渇時は NULL
 */
static xether_rx_zc_pbuf_t *xether_rx_zc_alloc(void)
{
    for (uint32_t i = 0U; i < NETC_HAL_RX_BUF_NUM; i++) {
        if (!s_rx_zc_in_use[i]) {
            s_rx_zc_in_use[i] = true;
            return &s_rx_zc_pool[i];
        }
    }
    return NULL;
}

/** ETH0〜ETH3（SWT）PHYハンドル */
static struct {
    phy_rtl8211f_resource_t resource;
    phy_handle_t             handle;
} g_xether_swt_phy[4U]; /* インデックス0〜3 = ETH0〜ETH3 */

/* ------------------------------------------------------------------ */
/* 内部ユーティリティ                                                  */
/* ------------------------------------------------------------------ */

/**
 * @brief ミリ秒待機（ビジーウェイト）
 * SDK_DelayAtLeastUs が利用できる環境ではそちらに置き換えること。
 */
static inline void xether_delay_ms(uint32_t ms)
{
    SDK_DelayAtLeastUs(ms * 1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

/** -------------------------------------------------------
	@brief	MIMXRT1180-EVKのETH4の初期化して通信できる状態にする
			・lwIPなどプロトコルスタックの初期化は行わない
	@param[in]	config	設定パラメータ情報
	@return	初期化結果
	@retval	TRUE	初期化成功
	@retval	FALSE	初期化失敗
-------------------------------------------------------- */
static bool_t xether_netc_ep0_open(const xether_config_t *config)
{
    if (config == NULL) {
        return (FALSE);
    }

    /* 二重オープン防止 */
    if (g_xether_board.opened == TRUE) {
        return (TRUE);
    }

    /* ----------------------------------------------------------------
     * NETC ENETC EP0 初期化
     *
     * クロック・PINMUXは xether_init_board() で完了済み前提。
     * ここではBDリング以外のENETC基本設定を行う。
     * ---------------------------------------------------------------- */
    netc_endpoint_config_t ep_cfg;
    (void)memset(&ep_cfg, 0, sizeof(ep_cfg));

    /* MACアドレス設定 */
    (void)memcpy(ep_cfg.si[0U].macAddr.addr, config->mac_addr, 6U);

    /* RGMIIポート設定（ETH4 = EP0ポート） */
    ep_cfg.port.ethMac.miiMode   = kNETC_RgmiiMode;
    ep_cfg.port.ethMac.miiSpeed  = kNETC_MiiSpeed1000M;
    ep_cfg.port.ethMac.miiDuplex = kNETC_MiiFullDuplex;

    status_t st = NETC_EnetcInit(&g_xether_board.ep_handle, &ep_cfg);
    if (st != kStatus_Success) {
        return (FALSE);
    }

    /* ----------------------------------------------------------------
     * PHY 初期化（RTL8211FDI-CG）
     *
     * MDIOアクセスリソースをPHYドライバに登録する。
     * xether_init_board() でMDIOコントローラは初期化済み。
     * ---------------------------------------------------------------- */
    g_xether_board.phy_resource.write = NETC_MDIOWrite;
    g_xether_board.phy_resource.read  = NETC_MDIORead;

    phy_config_t phy_cfg = {
        .phyAddr  = config->phy_addr,
        .autoNeg  = true,
        .speed    = kPHY_Speed1000M,
        .duplex   = kPHY_FullDuplex,
    };

    st = PHY_RTL8211F_Init(&g_xether_board.phy_handle,
                            &g_xether_board.phy_resource,
                            &phy_cfg);
    if (st != kStatus_Success) {
        return (FALSE);
    }

    /* ----------------------------------------------------------------
     * HAL 初期化（BDリング / DMAバッファセットアップ）
     *
     * ペリフェラル初期化済みの ep_handle を注入する。
     * ---------------------------------------------------------------- */
    netc_hal_init_params_t hal_params;
    (void)memset(&hal_params, 0, sizeof(hal_params));

    hal_params.ep_handle = &g_xether_board.ep_handle;
    hal_params.si_idx    = config->si_idx;
    (void)memcpy(hal_params.mac_addr, config->mac_addr, 6U);

    int ret = netc_hal_init(&hal_params, &g_xether_board.hal_ctx);
    if (ret != 0) {
        return (FALSE);
    }

    /* 初期リンクステータス取得 */
    g_xether_board.last_link_up = (bool_t)netc_hal_link_is_up(g_xether_board.hal_ctx);
    g_xether_board.opened       = TRUE;

    return (TRUE);
}

/** -------------------------------------------------------
	@brief	MIMXRT1180-EVKのETH4を停止して通信できない状態にする
			・lwIPなどプロトコルスタックの停止は行わない
	@param[in]	config	設定パラメータ情報
	@return	初期化結果
	@retval	TRUE	初期化成功
	@retval	FALSE	初期化失敗
-------------------------------------------------------- */
static void xether_netc_ep0_close(void)
{
    if (g_xether_board.opened == FALSE) {
        return;
    }

    /* HAL リソース解放（TX pending pbuf のクリア） */
    if (g_xether_board.hal_ctx != NULL) {
        netc_hal_deinit(g_xether_board.hal_ctx);
        g_xether_board.hal_ctx = NULL;
    }

    /* PHY をパワーダウンモードへ
     * RTL8211FではPHY_Write でパワーダウンビットを立てる。
     * SDKがPHY_PowerDown相当のAPIを提供していればそちらを使うこと。 */
    (void)PHY_RTL8211F_Write(&g_xether_board.phy_handle,
                              PHY_BASICCONTROL_REG,
                              PHY_BCTL_POWER_DOWN_MASK);

    /* ENETC EP0 を停止する */
    (void)NETC_EnetcDeinit(&g_xether_board.ep_handle);

    g_xether_board.last_link_up = FALSE;
    g_xether_board.opened       = FALSE;
}

/** -------------------------------------------------------
	@brief	MIMXRT1180-EVKのETH4から受信フレームを取得する
			・受信フレームが存在しない場合はすぐに応答を返す
			・lwIPのpbuf形式で取得する
	@return	pbuf形式の受信フレーム
	@retval	NULL以外	受信フレームあり
	@retval	NULL		受信フレームなし
-------------------------------------------------------- */
static struct pbuf *xether_netc_ep0_recv_packet_get(void)
{
    if (g_xether_board.opened == FALSE) {
        return (NULL);
    }

    /* HALから受信フレームをポーリング（ゼロコピー） */
    netc_hal_rx_frame_t rx_frame;
    if (!netc_hal_rx_poll(g_xether_board.hal_ctx, &rx_frame)) {
        /* 受信フレームなし */
        return (NULL);
    }

    /* ----------------------------------------------------------------
     * ゼロコピーRX: DMAバッファをコピーせず pbuf_alloced_custom() でラップ。
     *
     * DMAバッファ（s_rx_bufs[bd_idx]）はpbufのpayloadとして直接公開される。
     * lwIPスタックがpbufを使い終わって pbuf_free() を呼ぶと、
     * 参照カウントが 0 になった時点で xether_rx_pbuf_free_custom() が呼ばれ、
     * DMAバッファが自動的にHWのBDリングに返却される。
     * ---------------------------------------------------------------- */

    /* カスタムpbufスロットを確保する */
    xether_rx_zc_pbuf_t *slot = xether_rx_zc_alloc();
    if (slot == NULL) {
        /* プール枯渇: DMAバッファをHWに即返却してドロップ */
        netc_hal_rx_release(g_xether_board.hal_ctx, &rx_frame);
        return (NULL);
    }

    /* スロットにBD情報を保存（解放コールバックでHW返却に使う） */
    slot->rx_frame = rx_frame;
    slot->pbuf_custom.custom_free_function = xether_rx_pbuf_free_custom;

    /*
     * pbuf_alloced_custom():
     *   PBUF_REF  : 外部バッファを参照する（内部コピーなし）
     *   payload   : DMAバッファのアドレスをそのまま渡す
     *   tot_len   : 受信フレーム長
     *   max_len   : バッファの物理サイズ（NETC_HAL_MAX_FRAME_SIZE）
     */
    struct pbuf *p = pbuf_alloced_custom(
        PBUF_RAW,
        (uint16_t)rx_frame.length,
        PBUF_REF,
        &slot->pbuf_custom,
        rx_frame.data,
        NETC_HAL_MAX_FRAME_SIZE
    );

    if (p == NULL) {
        /* pbuf_alloced_custom 失敗: スロットとDMAバッファを両方返却 */
        s_rx_zc_in_use[slot - s_rx_zc_pool] = false;
        netc_hal_rx_release(g_xether_board.hal_ctx, &rx_frame);
        return (NULL);
    }

    return (p);
}

/** -------------------------------------------------------
	@brief	MIMXRT1180-EVKのETH4からパケットを送信する
	@param[in]	p		送信パケット
	@return		送信リクエスト結果
	@retval		TRUE	送信成功
	@retval		FALSE	送信失敗
-------------------------------------------------------- */
static bool_t xether_netc_ep0_send_packet_set(struct pbuf *p)
{
    if ((g_xether_board.opened == FALSE) || (p == NULL)) {
        return (FALSE);
    }

    /* TX reclaim：前回送信完了済みBDを回収してpbufを解放する */
    netc_hal_tx_reclaim(g_xether_board.hal_ctx);

    /* ----------------------------------------------------------------
     * ゼロコピーTX: pbuf->payload ポインタをBDに直接セットしてDMA送信。
     *
     * チェーンpbufはセグメントごとに個別にキューに積む。
     * DMA完了まで pbuf が解放されないよう pbuf_ref() で保護する。
     * ---------------------------------------------------------------- */
    for (struct pbuf *q = p; q != NULL; q = q->next) {
        /* DMA完了まで pbuf を保護 */
        pbuf_ref(q);

        netc_hal_tx_frame_t tx_frame = {
            .data   = (const uint8_t *)q->payload,
            .length = (uint16_t)q->len,
            .pbuf   = q,    /* reclaim時に pbuf_free() するために保持 */
        };

        int ret = netc_hal_tx_send(g_xether_board.hal_ctx, &tx_frame);
        if (ret != 0) {
            /* TXリング満杯: 参照カウントを戻して失敗返却 */
            pbuf_free(q);
            return (FALSE);
        }
    }

    return (TRUE);
}

/** -------------------------------------------------------
	@brief	MIMXRT1180-EVKのETH4のリンクステータスを更新する
			・PHYからリンクステータスを取得する
			・lwIPなどプロトコルスタックの状態は更新しない
	@return		リンクステータスの更新状況
	@retval		TRUE	更新あり
	@retval		FALSE	更新なし
-------------------------------------------------------- */
static bool_t xether_netc_ep0_link_status_update(void)
{
    if (g_xether_board.opened == FALSE) {
        return (FALSE);
    }

    /* PHYからリンク状態を直接取得する */
    bool link_up_raw = false;
    status_t st = PHY_RTL8211F_GetLinkStatus(&g_xether_board.phy_handle,
                                               &link_up_raw);
    if (st != kStatus_Success) {
        return (FALSE);
    }

    bool_t link_up = link_up_raw ? TRUE : FALSE;

    /* 前回値と比較して変化があれば更新 */
    if (link_up == g_xether_board.last_link_up) {
        return (FALSE);
    }

    g_xether_board.last_link_up = link_up;
    return (TRUE);
}

/** -------------------------------------------------------
	@brief MIMXRT1180-EVKのETH0,ETH1,ETH2,ETH3,ETH4を利用するために必要なGPIOとPHYを初期化する
-------------------------------------------------------- */
static inline void xether_init_board(void)
{
    /* ----------------------------------------------------------------
     * 1. PHY RESET GPIO 初期化
     *
     * EVKではGPIO13_IO16が全PHY（ETH0〜ETH4）共通のRESETピン。
     * アクティブLowのため、まずLowにしてリセットを印加する。
     * ---------------------------------------------------------------- */
    gpio_pin_config_t gpio_out_cfg = {
        .direction     = kGPIO_DigitalOutput,
        .outputLogic   = XETHER_PHY_RESET_ASSERT,   /* 初期値=Low（リセット中）*/
        .interruptMode = kGPIO_NoIntmode,
    };
    GPIO_PinInit(XETHER_PHY_RESET_GPIO, XETHER_PHY_RESET_PIN, &gpio_out_cfg);

    /* RESETパルス幅ウェイト（RTL8211F: tRSTLOW >= 10ms） */
    xether_delay_ms(XETHER_PHY_RESET_HOLD_MS);

    /* RESETをDeassert（High）してリセット解除 */
    GPIO_PinWrite(XETHER_PHY_RESET_GPIO, XETHER_PHY_RESET_PIN, XETHER_PHY_RESET_DEASSERT);

    /* PHY安定待ち（RTL8211F: tRSTHOLD >= 30ms） */
    xether_delay_ms(XETHER_PHY_RESET_STABLE_MS);

    /* ----------------------------------------------------------------
     * 2. NETC EMDIO コントローラ初期化
     *
     * ETH0〜ETH4全PHYのMDIOアクセスに共通使用する。
     * クロック設定はclock_config.c / BOARD_InitNETCClocks() 済み前提。
     * ---------------------------------------------------------------- */
    netc_mdio_config_t mdio_cfg = {
        .mdioClkHz  = 2500000U,   /* MDIO clock: 2.5 MHz（IEEE802.3規定上限）*/
        .isPreamble = true,
    };
    (void)NETC_MDIOInit(&mdio_cfg);

    /* ----------------------------------------------------------------
     * 3. ETH0〜ETH3（NETC SWT PORT0〜3）のPHY初期化
     *
     * スイッチポートのMACは NETC_SwtInit() で設定する。
     * ここではPHYのリンクアップのみ行い、SWT本体の設定はアプリ責務とする。
     * ---------------------------------------------------------------- */
    static const uint32_t swt_phy_addr[4U] = {
        XETHER_ETH0_PHY_ADDR,
        XETHER_ETH1_PHY_ADDR,
        XETHER_ETH2_PHY_ADDR,
        XETHER_ETH3_PHY_ADDR,
    };

    for (uint32_t i = 0U; i < 4U; i++) {
        g_xether_swt_phy[i].resource.write = NETC_MDIOWrite;
        g_xether_swt_phy[i].resource.read  = NETC_MDIORead;

        phy_config_t phy_cfg = {
            .phyAddr  = swt_phy_addr[i],
            .autoNeg  = true,
            .speed    = kPHY_Speed1000M,
            .duplex   = kPHY_FullDuplex,
        };

        /* 初期化失敗は無視してアサーション等で検出させる
         * （ここで止めると他ポートの初期化まで影響するため）*/
        (void)PHY_RTL8211F_Init(&g_xether_swt_phy[i].handle,
                                 &g_xether_swt_phy[i].resource,
                                 &phy_cfg);
    }

    /* ----------------------------------------------------------------
     * 4. ETH4（NETC EP0）のENETC基本クロック有効化
     *
     * BOARD_InitNETCClocks() が未実施の場合はここで呼ぶ。
     * SDK のボードファイルで定義されている前提とし、ここでは
     * 呼び出しのみ行う。
     * ---------------------------------------------------------------- */
    BOARD_InitNETCClocks();

    /* ----------------------------------------------------------------
     * 5. モジュール内部状態のゼロクリア
     * ---------------------------------------------------------------- */
    (void)memset(&g_xether_board, 0, sizeof(g_xether_board));
}

/** -------------------------------------------------------
	@brief MIMXRT1180-EVKのETH0,ETH1,ETH2,ETH3,ETH4に関するGPIOとPHYを安全に停止するための処理
-------------------------------------------------------- */
static inline void xether_deinit_board(void)
{
    /* ----------------------------------------------------------------
     * 1. ETH4（EP0）が開いていれば閉じる
     * ---------------------------------------------------------------- */
    if (g_xether_board.opened == TRUE) {
        xether_netc_ep0_close();
    }

    /* ----------------------------------------------------------------
     * 2. ETH0〜ETH3（SWT PORT0〜3）のPHYをパワーダウン
     * ---------------------------------------------------------------- */
    for (uint32_t i = 0U; i < 4U; i++) {
        (void)PHY_RTL8211F_Write(&g_xether_swt_phy[i].handle,
                                  PHY_BASICCONTROL_REG,
                                  PHY_BCTL_POWER_DOWN_MASK);
    }

    /* ----------------------------------------------------------------
     * 3. PHY RESET GPIO をLow（リセット状態）に戻す
     *
     * 全PHYをリセット状態に落とすことで消費電流を抑える。
     * ---------------------------------------------------------------- */
    GPIO_PinWrite(XETHER_PHY_RESET_GPIO, XETHER_PHY_RESET_PIN, XETHER_PHY_RESET_ASSERT);

    /* ----------------------------------------------------------------
     * 4. MDIO コントローラ停止
     * ---------------------------------------------------------------- */
    NETC_MDIODeinit();

    /* ----------------------------------------------------------------
     * 5. 内部状態クリア
     * ---------------------------------------------------------------- */
    (void)memset(&g_xether_board, 0, sizeof(g_xether_board));
    (void)memset(g_xether_swt_phy, 0, sizeof(g_xether_swt_phy));
}


XETHERNET_DEVICE_LIST_BEGIN()
  XETHERNET_DEVICE_LIST_ITEM(xether_netc_ep0),
XETHERNET_DEVICE_LIST_END()

#endif /* XETHER_BOARD_H_ */
