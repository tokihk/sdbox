/*
 * xethernet_board.h
 *
 *  Created on: 2024/03/27
 *      Author: Hitoshi Kono
 */

#ifndef XETHERNET_BOARD_H_
#define XETHERNET_BOARD_H_


#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"


/* PHYのMDIOアドレス */
/* TODO: 実機のPHYストラップ(AD0/AD1ピン)設定に合わせて確認すること */
#define XETHER_EP0_PHY_ADDR					(0x01U)

/* NETC MDIOのソースクロック */
/* TODO: 実際のNETC/MDIOのソースクロック周波数に合わせて確認すること */
#define XETHER_EP0_MDIO_SRC_CLOCK_HZ			(200000000U)

/* IEEE802.3 標準PHYレジスタ (RTL8201FIも準拠) */
#define XETHER_PHY_REG_BCR						(0x00U)		/* Basic Control Register */
#define XETHER_PHY_REG_BSR						(0x01U)		/* Basic Status Register */
#define XETHER_PHY_REG_ANAR						(0x04U)		/* Auto-Negotiation Advertisement */
#define XETHER_PHY_REG_ANLPAR					(0x05U)		/* Auto-Negotiation Link Partner Ability */

#define XETHER_PHY_BCR_RESET					(0x8000U)
#define XETHER_PHY_BCR_AUTONEG_EN				(0x1000U)
#define XETHER_PHY_BCR_POWER_DOWN				(0x0800U)
#define XETHER_PHY_BCR_RESTART_AUTONEG			(0x0200U)

#define XETHER_PHY_BSR_AUTONEG_COMPLETE			(0x0020U)
#define XETHER_PHY_BSR_LINK_STATUS				(0x0004U)

#define XETHER_PHY_ANAR_100_FULL				(0x0100U)
#define XETHER_PHY_ANAR_100_HALF				(0x0080U)
#define XETHER_PHY_ANAR_10_FULL					(0x0040U)
#define XETHER_PHY_ANAR_10_HALF					(0x0020U)
#define XETHER_PHY_ANAR_802_3					(0x0001U)

#define XETHER_PHY_RESET_TIMEOUT_COUNT			(1000U)

/* 送受信リングの長さ、バッファサイズ */
#define XETHER_EP0_RX_RING_LEN					(8U)
#define XETHER_EP0_TX_RING_LEN					(8U)
#define XETHER_EP0_RX_BUFF_SIZE					(1536U)
#define XETHER_EP0_TX_MAX_SEGMENT				(8U)		/* pbufチェインの最大分割数 */


/* NETC/PHY/MDIOのハンドルとリソース(BD/バッファ)は非キャッシュ領域に配置する */
static ep_handle_t							s_xether_ep0_handle;
static netc_mdio_handle_t					s_xether_ep0_mdio_handle;
static bool_t								s_xether_ep0_is_opened = FALSE;

XSECTION_NONCACHE_DATA(static netc_rx_bd_t		s_xether_ep0_rx_bd[XETHER_EP0_RX_RING_LEN]);
XSECTION_NONCACHE_DATA(static netc_tx_bd_t		s_xether_ep0_tx_bd[XETHER_EP0_TX_RING_LEN]);
XSECTION_NONCACHE_DATA(static uint8_t			s_xether_ep0_rx_buff[XETHER_EP0_RX_RING_LEN][XETHER_EP0_RX_BUFF_SIZE]);
static uint8_t								*s_xether_ep0_rx_buff_addr[XETHER_EP0_RX_RING_LEN];

/* 受信フレームをpbufへコピーするための一時バッファ */
static uint8_t								s_xether_ep0_rx_scratch[XETHER_EP0_RX_BUFF_SIZE];


static struct
{
	bool_t						last_link_up;		/* 最新のリンクステータス */
} g_xether_board;


/** -------------------------------------------------------
	@brief	RTL8201FI-VC-CGのレジスタをMDIO経由で読み出す
	@param[in]	reg_addr	レジスタアドレス
	@param[out]	data		読み出したデータの格納先
	@return	読み出し結果
	@retval	TRUE	成功
	@retval	FALSE	失敗
-------------------------------------------------------- */
static bool_t xether_ep0_phy_read(uint8_t reg_addr, uint16_t *data)
{
	return ((NETC_MDIORead(&s_xether_ep0_mdio_handle, XETHER_EP0_PHY_ADDR, reg_addr, data) == kStatus_Success) ? TRUE : FALSE);
}


/** -------------------------------------------------------
	@brief	RTL8201FI-VC-CGのレジスタをMDIO経由で書き込む
	@param[in]	reg_addr	レジスタアドレス
	@param[in]	data		書き込むデータ
	@return	書き込み結果
	@retval	TRUE	成功
	@retval	FALSE	失敗
-------------------------------------------------------- */
static bool_t xether_ep0_phy_write(uint8_t reg_addr, uint16_t data)
{
	return ((NETC_MDIOWrite(&s_xether_ep0_mdio_handle, XETHER_EP0_PHY_ADDR, reg_addr, data) == kStatus_Success) ? TRUE : FALSE);
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
	netc_bdr_config_t			bdr_config;
	netc_rx_bdr_config_t		rx_bdr_config;
	netc_tx_bdr_config_t		tx_bdr_config;
	ep_config_t					ep_config;
	netc_mdio_config_t			mdio_config;
	uint16_t					bcr;
	uint32_t					i;
	uint32_t					timeout;

	if ((config == NULL) || (s_xether_ep0_is_opened != FALSE))
	{
		return (FALSE);
	}

	(void)memset(&bdr_config, 0, sizeof(bdr_config));
	(void)memset(&rx_bdr_config, 0, sizeof(rx_bdr_config));
	(void)memset(&tx_bdr_config, 0, sizeof(tx_bdr_config));
	(void)memset(&mdio_config, 0, sizeof(mdio_config));

	/* --- MDIO(PHYとのSMI通信バス)の初期化 --- */
	mdio_config.srcClockHz = XETHER_EP0_MDIO_SRC_CLOCK_HZ;

	if (NETC_MDIOInit(&s_xether_ep0_mdio_handle, &mdio_config) != kStatus_Success)
	{
		return (FALSE);
	}

	/* --- PHY(RTL8201FI-VC-CG)のソフトウェアリセット --- */
	if (xether_ep0_phy_write(XETHER_PHY_REG_BCR, XETHER_PHY_BCR_RESET) == FALSE)
	{
		return (FALSE);
	}

	timeout = 0;
	bcr = XETHER_PHY_BCR_RESET;
	while (((bcr & XETHER_PHY_BCR_RESET) != 0U) && (timeout < XETHER_PHY_RESET_TIMEOUT_COUNT))
	{
		if (xether_ep0_phy_read(XETHER_PHY_REG_BCR, &bcr) == FALSE)
		{
			return (FALSE);
		}
		timeout++;
	}

	if ((bcr & XETHER_PHY_BCR_RESET) != 0U)
	{
		/* リセットが完了しなかった */
		return (FALSE);
	}

	/* --- オートネゴシエーションの開始(10/100M, 全二重/半二重を許容) --- */
	(void)xether_ep0_phy_write(XETHER_PHY_REG_ANAR,
								(uint16_t)(XETHER_PHY_ANAR_100_FULL | XETHER_PHY_ANAR_100_HALF |
										   XETHER_PHY_ANAR_10_FULL  | XETHER_PHY_ANAR_10_HALF  |
										   XETHER_PHY_ANAR_802_3));
	(void)xether_ep0_phy_write(XETHER_PHY_REG_BCR,
								(uint16_t)(XETHER_PHY_BCR_AUTONEG_EN | XETHER_PHY_BCR_RESTART_AUTONEG));

	/* --- 受信リングの初期化(ディスクリプタとバッファアドレスの紐付け) --- */
	for (i = 0; i < XETHER_EP0_RX_RING_LEN; i++)
	{
		s_xether_ep0_rx_buff_addr[i] = &s_xether_ep0_rx_buff[i][0];
	}

	/*
	 * TODO: netc_rx_bdr_config_t / netc_tx_bdr_config_t のメンバ名は
	 *       実際の fsl_netc_endpoint.h の定義に合わせて確認・修正すること。
	 *       (bdArray, len, buffAddrArray, buffSize 等は代表的な命名から推測したもの)
	 */
	rx_bdr_config.bdArray       = &s_xether_ep0_rx_bd[0];
	rx_bdr_config.len           = XETHER_EP0_RX_RING_LEN;
	rx_bdr_config.buffAddrArray = &s_xether_ep0_rx_buff_addr[0];
	rx_bdr_config.buffSize      = XETHER_EP0_RX_BUFF_SIZE;
	rx_bdr_config.extendDescEn  = false;

	tx_bdr_config.bdArray       = &s_xether_ep0_tx_bd[0];
	tx_bdr_config.len           = XETHER_EP0_TX_RING_LEN;

	bdr_config.rxBdrConfig[0] = rx_bdr_config;
	bdr_config.txBdrConfig[0] = tx_bdr_config;

	/* --- NETCエンドポイント(ETH4/EP0)本体の初期化 --- */
	EP_GetDefaultConfig(&ep_config);
	ep_config.si                    = 0U;
	ep_config.siConfig.txRingUse     = 1U;
	ep_config.siConfig.rxRingUse     = 1U;
	/* RTL8201FI-VC-CGは10/100M専用PHYのためRMII接続を使用する */
	ep_config.port.ethMac.miiMode    = kNETC_RmiiMode;
	ep_config.port.ethMac.miiSpeed   = kNETC_MiiSpeed100M;
	ep_config.port.ethMac.miiDuplex  = kNETC_MiiFullDuplex;

	if (EP_Init(&s_xether_ep0_handle, (uint8_t *)&config->mac_addr.addr[0], &ep_config, &bdr_config) != kStatus_Success)
	{
		return (FALSE);
	}

	g_xether_board.last_link_up = FALSE;
	s_xether_ep0_is_opened = TRUE;

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
	if (s_xether_ep0_is_opened == FALSE)
	{
		return;
	}

	/* PHYをパワーダウン状態にする */
	(void)xether_ep0_phy_write(XETHER_PHY_REG_BCR, XETHER_PHY_BCR_POWER_DOWN);

	/* NETCエンドポイントの終了処理 */
	/* TODO: 関数名(EP_Deinit)は fsl_netc_endpoint.h の実際の宣言に合わせて確認すること */
	(void)EP_Deinit(&s_xether_ep0_handle);

	s_xether_ep0_is_opened = FALSE;
	g_xether_board.last_link_up = FALSE;
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
	netc_frame_attr_t			attr;
	struct pbuf					*p;
	uint32_t					length;
	status_t					rx_result;

	if (s_xether_ep0_is_opened == FALSE)
	{
		return (NULL);
	}

	(void)memset(&attr, 0, sizeof(attr));

	/* 受信フレームの有無とサイズを確認する。フレームが無ければ即座に応答する */
	rx_result = EP_GetRxFrameSize(&s_xether_ep0_handle, 0U, &length);
	if (rx_result == kStatus_NETC_RxFrameEmpty)
	{
		return (NULL);
	}

	if ((rx_result != kStatus_Success) || (length == 0U) || (length > sizeof(s_xether_ep0_rx_scratch)))
	{
		/* 異常なフレームはリングから読み捨てる */
		(void)EP_ReceiveFrameCopy(&s_xether_ep0_handle, 0U, s_xether_ep0_rx_scratch,
								   sizeof(s_xether_ep0_rx_scratch), &attr);
		return (NULL);
	}

	if (EP_ReceiveFrameCopy(&s_xether_ep0_handle, 0U, s_xether_ep0_rx_scratch, length, &attr) != kStatus_Success)
	{
		return (NULL);
	}

	p = pbuf_alloc(PBUF_RAW, (u16_t)length, PBUF_POOL);
	if (p == NULL)
	{
		/* pbuf確保に失敗した場合はフレームを読み捨てて終了する */
		return (NULL);
	}

	if (pbuf_take(p, s_xether_ep0_rx_scratch, (u16_t)length) != (err_t)ERR_OK)
	{
		(void)pbuf_free(p);
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
	netc_buffer_struct_t		buff_list[XETHER_EP0_TX_MAX_SEGMENT];
	netc_frame_struct_t			frame;
	ep_tx_opt					opt;
	struct pbuf					*q;
	uint32_t					seg_num;
	status_t					result;

	if ((s_xether_ep0_is_opened == FALSE) || (p == NULL))
	{
		return (FALSE);
	}

	(void)memset(&opt, 0, sizeof(opt));

	/* pbufチェイン(複数セグメント)をNETCのバッファリストへ変換する */
	seg_num = 0;
	for (q = p; q != NULL; q = q->next)
	{
		if (seg_num >= XETHER_EP0_TX_MAX_SEGMENT)
		{
			/* バッファリストの上限を超えるフレームは送信できない */
			return (FALSE);
		}

		buff_list[seg_num].buffer = (uint8_t *)q->payload;
		buff_list[seg_num].length = (uint16_t)q->len;
		seg_num++;
	}

	if (seg_num == 0U)
	{
		return (FALSE);
	}

	frame.buffArray = &buff_list[0];
	frame.length    = (uint8_t)seg_num;

	result = EP_SendFrame(&s_xether_ep0_handle, 0U, &frame, NULL, &opt);

	return ((result == kStatus_Success) ? TRUE : FALSE);
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
	uint16_t					bsr;
	uint16_t					anar;
	uint16_t					anlpar;
	uint16_t					common_ability;
	bool_t						link_up;

	if (s_xether_ep0_is_opened == FALSE)
	{
		return (FALSE);
	}

	/*
	 * BSR(Basic Status Register)のLink Statusビットはラッチ動作のため、
	 * 最新の状態を得るために2回連続で読み出す(標準的なPHYの作法)。
	 */
	if (xether_ep0_phy_read(XETHER_PHY_REG_BSR, &bsr) == FALSE)
	{
		return (FALSE);
	}
	if (xether_ep0_phy_read(XETHER_PHY_REG_BSR, &bsr) == FALSE)
	{
		return (FALSE);
	}

	link_up = ((bsr & XETHER_PHY_BSR_LINK_STATUS) != 0U) ? TRUE : FALSE;

	if (link_up == g_xether_board.last_link_up)
	{
		/* 変化なし */
		return (FALSE);
	}

	if ((link_up != FALSE) && ((bsr & XETHER_PHY_BSR_AUTONEG_COMPLETE) != 0U))
	{
		netc_hw_mii_speed_t			mii_speed  = kNETC_MiiSpeed100M;
		netc_hw_mii_duplex_t			mii_duplex = kNETC_MiiFullDuplex;

		/* 自ノードと相手ノードの共通のオートネゴシエーション能力から速度・全二重/半二重を決定する */
		if ((xether_ep0_phy_read(XETHER_PHY_REG_ANAR, &anar) != FALSE) &&
			(xether_ep0_phy_read(XETHER_PHY_REG_ANLPAR, &anlpar) != FALSE))
		{
			common_ability = (uint16_t)(anar & anlpar);

			if ((common_ability & XETHER_PHY_ANAR_100_FULL) != 0U)
			{
				mii_speed  = kNETC_MiiSpeed100M;
				mii_duplex = kNETC_MiiFullDuplex;
			}
			else if ((common_ability & XETHER_PHY_ANAR_100_HALF) != 0U)
			{
				mii_speed  = kNETC_MiiSpeed100M;
				mii_duplex = kNETC_MiiHalfDuplex;
			}
			else if ((common_ability & XETHER_PHY_ANAR_10_FULL) != 0U)
			{
				mii_speed  = kNETC_MiiSpeed10M;
				mii_duplex = kNETC_MiiFullDuplex;
			}
			else
			{
				mii_speed  = kNETC_MiiSpeed10M;
				mii_duplex = kNETC_MiiHalfDuplex;
			}
		}

		/* NETC側のMAC速度・デュプレックス設定をリンクアップ後の値に合わせる */
		(void)EP_Up(&s_xether_ep0_handle, mii_speed, mii_duplex);
	}

	g_xether_board.last_link_up = link_up;

	return (TRUE);
}

/** -------------------------------------------------------
	@brief MIMXRT1180-EVKのETH0,ETH1,ETH2,ETH3,ETH4を利用するために必要なGPIOとPHYを初期化する
-------------------------------------------------------- */
static inline void xether_init_board(void)
{
	/*
	 * TODO: 以下はテンプレートである。MIMXRT1180-EVKの回路図およびMCUXpresso Config Tools
	 *       (pin_mux.c / clock_config.c) が生成する実際のピン名・GPIOポート・クロック名に
	 *       置き換えること。
	 *
	 *       想定している処理内容:
	 *       1) NETC/GPIO関連クロックの有効化
	 *       2) ETH0~ETH4用ピン(RGMII/RMII/MDIO)のピンマルチプレクス設定
	 *       3) 各PHYのリセットピンをGPIO出力として初期化し、リセットパルスを与える
	 *          (Low >= 10ms 保持 -> High、RTL8201FIのデータシートのリセットタイミング要件に従う)
	 */

	/* 1) クロック有効化 */
	CLOCK_EnableClock(kCLOCK_Netc);						/* TODO: 実際のクロック名に置き換える */

	/* 2) ピンマルチプレクス設定 (MCUXpresso Config Toolsで生成される関数を想定) */
	BOARD_InitPins();										/* TODO: 実際の関数名に置き換える */

	/* 3) ETH4 (RTL8201FI-VC-CG) のPHYリセット */
	{
		gpio_pin_config_t		reset_pin_config;

		reset_pin_config.direction     = kGPIO_DigitalOutput;
		reset_pin_config.outputLogic   = 0U;				/* リセット(Low)状態から開始 */
		reset_pin_config.interruptMode = kGPIO_NoIntmode;

		/* TODO: BOARD_ETH4_PHY_RST_GPIO / BOARD_ETH4_PHY_RST_PIN は実際の定義に置き換える */
		GPIO_PinInit(BOARD_ETH4_PHY_RST_GPIO, BOARD_ETH4_PHY_RST_PIN, &reset_pin_config);

		GPIO_PinWrite(BOARD_ETH4_PHY_RST_GPIO, BOARD_ETH4_PHY_RST_PIN, 0U);
		SDK_DelayAtLeastUs(10000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
		GPIO_PinWrite(BOARD_ETH4_PHY_RST_GPIO, BOARD_ETH4_PHY_RST_PIN, 1U);
		SDK_DelayAtLeastUs(10000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
	}
}

/** -------------------------------------------------------
	@brief MIMXRT1180-EVKのETH0,ETH1,ETH2,ETH3,ETH4に関するGPIOとPHYを安全に停止するための処理
-------------------------------------------------------- */
static inline void xether_deinit_board(void)
{
	/* 各PHYをリセット状態(Low)に戻してから、クロックを止める */
	/* TODO: BOARD_ETH4_PHY_RST_GPIO / BOARD_ETH4_PHY_RST_PIN は実際の定義に置き換える */
	GPIO_PinWrite(BOARD_ETH4_PHY_RST_GPIO, BOARD_ETH4_PHY_RST_PIN, 0U);

	CLOCK_DisableClock(kCLOCK_Netc);						/* TODO: 実際のクロック名に置き換える */
}


XETHERNET_DEVICE_LIST_BEGIN()
  XETHERNET_DEVICE_LIST_ITEM(xether_netc_ep0),
XETHERNET_DEVICE_LIST_END()

#endif /* XETHER_BOARD_H_ */
