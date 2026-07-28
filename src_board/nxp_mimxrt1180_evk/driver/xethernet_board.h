/*
 * xethernet_board.h
 *
 *  Created on: 2024/03/27
 *      Author: Hitoshi Kono
 */

#ifndef XETHERNET_BOARD_H_
#define XETHERNET_BOARD_H_


static struct
{
	bool_t						last_link_up;		/* 最新のリンクステータス */
} g_xether_board;


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
	return (NULL);
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
	return (FALSE);
}

/** -------------------------------------------------------
	@brief MIMXRT1180-EVKのETH0,ETH1,ETH2,ETH3,ETH4を利用するために必要なGPIOとPHYを初期化する
-------------------------------------------------------- */
static inline void xether_init_board(void)
{
}

/** -------------------------------------------------------
	@brief MIMXRT1180-EVKのETH0,ETH1,ETH2,ETH3,ETH4に関するGPIOとPHYを安全に停止するための処理
-------------------------------------------------------- */
static inline void xether_deinit_board(void)
{
}


XETHERNET_DEVICE_LIST_BEGIN()
  XETHERNET_DEVICE_LIST_ITEM(xether_netc_ep0),
XETHERNET_DEVICE_LIST_END()

#endif /* XETHER_BOARD_H_ */
