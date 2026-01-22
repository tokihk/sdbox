/*
 * xinternet.h
 *
 *  Created on: 2024/04/03
 *      Author: Hitoshi Kono
 */

#ifndef XINTERNET_H_
#define XINTERNET_H_

#include "core/xstddef.h"

#include "lwip/udp.h"
#include "lwip/prot/ethernet.h"


typedef struct xinet_udp_object xinet_udp_object_t;


typedef void (*xinet_udp_recv_func_t)(xinet_udp_object_t *obj, struct pbuf *p, const ip_addr_t *remote_addr, u16_t remote_port);


typedef struct xinet_udp_config
{
	ip_addr_t					local_addr;
	uint16_t					local_port;

	xinet_udp_recv_func_t		recv_func;
} xinet_udp_config_t;

typedef struct xinet_udp_object
{
	xinet_udp_config_t			config;
	struct udp_pcb *			pcb;
} xinet_udp_object_t;


bool_t							xinet_udp_open(xinet_udp_object_t *udp_obj, const xinet_udp_config_t *config);
void							xinet_udp_close(xinet_udp_object_t *udp_obj);

bool_t							xinet_udp_send(xinet_udp_object_t *udp_obj, const ip_addr_t *remote_addr, u16_t remote_port, struct pbuf *pkt);


#endif /* XINTERNET_H_ */
