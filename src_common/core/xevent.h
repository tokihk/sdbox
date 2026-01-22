/*
 * xevent.h
 *
 *  Created on: 2024/05/03
 *      Author: Hitoshi Kono
 */

#ifndef XEVENT_H_
#define XEVENT_H_

#include "core/xstddef.h"


typedef void *					xevent_group_t;


xevent_group_t					xevent_group_new(void);
void							xevent_group_delete(xevent_group_t handle);

uint32_t						xevent_group_flags_clear(xevent_group_t handle, uint32_t flags);
uint32_t						xevent_group_flags_get(xevent_group_t handle);
uint32_t						xevent_group_flags_set(xevent_group_t handle, uint32_t flags);
uint32_t						xevent_group_flags_wait(xevent_group_t handle, uint32_t flags, bool_t flags_clear, bool_t wait_all, uint32_t timeout_msec);

uint32_t						xevent_group_flags_clear_isr(xevent_group_t handle, uint32_t flags);
uint32_t						xevent_group_flags_get_isr(xevent_group_t handle);
uint32_t						xevent_group_flags_set_isr(xevent_group_t handle, uint32_t flags);


#endif /* XEVENT_H_ */
