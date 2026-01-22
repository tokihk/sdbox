/*
 * xinternet.c
 *
 *  Created on: 2024/04/03
 *      Author: Hitoshi Kono
 */

#include "xinternet.h"

#include "core/xmalloc.h"

#include "driver/xethernet.h"

#include "lwip/init.h"
#include "lwip/etharp.h"
#include "netif/ethernet.h"


static void xinet_lwip_udp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	xinet_udp_object_t *udp_obj = (xinet_udp_object_t *)arg;

	(udp_obj->config.recv_func)(udp_obj, p, addr, port);

	pbuf_free(p);
}

void xinet_init(void)
{
}

void xinet_deinit(void)
{

}

bool_t xinet_udp_open(xinet_udp_object_t *udp_obj, const xinet_udp_config_t *config)
{
	bool_t open_ok = FALSE;

	if (   (udp_obj != NULL)
		&& (config != NULL)
	) {
		udp_obj->config = *config;

		/* PCB生成 */
		udp_obj->pcb = udp_new();
		if (udp_obj->pcb != NULL)
		{
			/* バインド */
			if (udp_bind(udp_obj->pcb, &(udp_obj->config.local_addr), udp_obj->config.local_port) == ERR_OK)
			{
				if (udp_obj->config.recv_func != NULL)
				{
					/* 受信開始 */
					udp_recv(udp_obj->pcb, xinet_lwip_udp_recv, udp_obj);
				}

				open_ok = TRUE;
			}
		}
	}

	if (!open_ok)
	{
		xinet_udp_close(udp_obj);
	}

	return (open_ok);
}

void xinet_udp_close(xinet_udp_object_t *udp_obj)
{
	if (udp_obj != NULL)
	{
		if (udp_obj->pcb != NULL)
		{
			udp_remove(udp_obj->pcb);

			udp_obj->pcb = NULL;
		}
	}
}

bool_t xinet_udp_send(xinet_udp_object_t *udp_obj, const ip_addr_t *remote_addr, u16_t remote_port, struct pbuf *pkt)
{
	bool_t send_ok = FALSE;

	if (   (udp_obj != NULL)
		&& (remote_addr != NULL)
		&& (pkt != NULL)
	) {
		if (udp_sendto(udp_obj->pcb, pkt, remote_addr, remote_port) == ERR_OK)
		{
			send_ok = TRUE;
		}
	}

	return (send_ok);
}

