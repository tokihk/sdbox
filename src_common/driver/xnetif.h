/*
 * xnetif.h
 *
 *  Created on: 2024/10/15
 *      Author: Hitoshi Kono
 */

#ifndef XNETIF_H_
#define XNETIF_H_

#include "core/xstddef.h"

#include "driver/xethernet.h"
#include "driver/xinternet.h"

typedef struct xnetif_config
{
	struct eth_addr				mac_addr;

	ip4_addr_t					ipv4_addr;
	ip4_addr_t					ipv4_netmask;
	ip4_addr_t					ipv4_gateway;

	uint16_t					mtu;
} xnetif_config_t;

typedef struct xnetif_status
{
	uint32_t					tx_packet_count;
	uint32_t					rx_packet_count;

	bool_t						is_opened;
	bool_t						is_linkup;
} xnetif_status_t;


void							xnetif_init(void);
void							xnetif_deinit(void);
void							xnetif_poll(void);

bool_t							xnetif_open(uint16_t id, const xnetif_config_t *config);
void							xnetif_close(uint16_t id);

bool_t							xnetif_config_get(uint16_t id, xnetif_config_t *config);

void							xnetif_status_reset(uint16_t id);
bool_t							xnetif_status_get(uint16_t id, xnetif_status_t *status);

#endif /* XNETIF_H_ */
