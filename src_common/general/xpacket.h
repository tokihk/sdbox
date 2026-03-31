/*
 * xpacket.h
 *
 *  Created on: 2026/03/31
 *      Author: H.Kono
 */

#ifndef GENERAL_XPACKET_H_
#define GENERAL_XPACKET_H_

#include "core/xstddef.h"


struct xpacket;


typedef void			(* xpacket_free_func_t)(struct xpacket *packet);


typedef struct xpacket
{
	struct xpacket *			next;

	uint8_t *					payload;
	uint16_t					payload_size;

	uint16_t					payload_total_size;

	xpacket_free_func_t			free_func;
} xpacket_t;


#define xpacket_placement_size(payload_size)	(sizeof(struct xpacket) + (payload_size))


xpacket_t *						xpacket_new_
xpacket_t *						xpacket_new_custom(uint8_t *buffer, uint16_t buffer_size, xpacket_free_func_t free_func);

void							xpacket_delete(xpacket_t *packet);


#endif /* GENERAL_XPACKET_H_ */
