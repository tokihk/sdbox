/*
 * xethernet_board.h
 *
 *  Created on: 2024/03/27
 *      Author: Hitoshi Kono
 */

#ifndef XETHERNET_BOARD_H_
#define XETHERNET_BOARD_H_


static bool_t xether_netc_ep0_open(const xether_config_t *config)
{
	return (TRUE);
}

static void xether_netc_ep0_close(void)
{
}

static struct pbuf *xether_netc_ep0_recv_packet_get(void)
{
	return (NULL);
}

static bool_t xether_netc_ep0_send_packet_set(struct pbuf *p)
{
	return (TRUE);
}

static bool_t xether_netc_ep0_link_status_update(void)
{
	return (FALSE);
}

static inline void xether_init_board(void)
{
}

static inline void xether_deinit_board(void)
{
}


XETHERNET_DEVICE_LIST_BEGIN()
  XETHERNET_DEVICE_LIST_ITEM(xether_netc_ep0),
XETHERNET_DEVICE_LIST_END()

#endif /* XETHER_BOARD_H_ */
