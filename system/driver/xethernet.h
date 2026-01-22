/*
 * xethernet.h
 *
 *  Created on: 2024/03/27
 *      Author: Hitoshi Kono
 */

#ifndef XETHERNET_H_
#define XETHERNET_H_

#include "core/xstddef.h"

#include "lwip/prot/ethernet.h"
#include "lwip/pbuf.h"


typedef struct xether_config
{
	struct eth_addr				mac_addr;
} xether_config_t;

typedef struct xether_status
{
	uint32_t					rx_packet_count;
	uint32_t					tx_packet_count;

	bool_t						is_opened;
	bool_t						is_linkup;
} xether_status_t;


void							xether_init(void);
void							xether_deinit(void);

uint16_t						xether_device_num_get(void);

bool_t							xether_open(uint16_t id, const xether_config_t *config);
void							xether_close(uint16_t id);

bool_t							xether_config_get(uint16_t id, xether_config_t *config);

bool_t							xether_link_status_update(uint16_t id);

struct pbuf *					xether_recv_packet_get(uint16_t id);
bool_t							xether_send_packet_set(uint16_t id, struct pbuf *pkt);


#endif /* XETHERNET_H_ */
