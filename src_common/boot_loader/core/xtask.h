/*
 * xtask.h
 *
 *  Created on: 2022/12/24
 *      Author: Hitoshi Kono
 */

#ifndef XTASK_H_
#define XTASK_H_

#include "core/xstddef.h"


#if XCORE_CONF_OS_TYPE == XCORE_CONF_OS_FREE_RTOS

typedef struct xtask_config
{
	const char *				name;
	uint32_t					stack_size;
	uint16_t					prio;
} xtask_config_t;


typedef void *					xtask_t;
typedef void *					xmutex_t;
typedef void *					xsemaphore_t;


typedef void					(* xtask_func_t)(xtask_t handle, void *param1, void *param2);


void							xtask_init(xtask_func_t main_task_func, uint32_t stack_size, uint16_t priority, const char *name, void *param1, void *param2);

uint32_t						xtask_tick_get(void);
uint32_t						xtask_tick_diff(uint32_t prev_tick, uint32_t curr_tick);

void							xtask_lock(void);
void							xtask_unlock(void);

xtask_t							xtask_new(xtask_func_t func, uint32_t stack_size, uint16_t priority, const char *name, void *param1, void *param2);
xtask_t							xtask_new_config(xtask_func_t func, const xtask_config_t *config, void *param1, void *param2);
void							xtask_delete(xtask_t handle);

xtask_t							xtask_current_get(void);

void							xtask_delay(uint32_t wait_ms);
void							xtask_delay_until(uint32_t *cur_tick, uint32_t wait_msec);

xmutex_t						xmutex_new(void);
void							xmutex_delete(xmutex_t handle);
bool_t							xmutex_lock(xmutex_t handle, uint32_t timeout_msec);
bool_t							xmutex_lock_isr(xmutex_t handle);
void							xmutex_unlock(xmutex_t handle);
void							xmutex_unlock_isr(xmutex_t handle);

xsemaphore_t					xsemaphore_new(uint32_t max_count, uint32_t initial_count);
void							xsemaphore_delete(xsemaphore_t handle);
uint32_t						xsemaphore_count_get(xsemaphore_t handle);
uint32_t						xsemaphore_count_get_isr(xsemaphore_t handle);
bool_t							xsemaphore_lock(xsemaphore_t handle, uint32_t timeout_msec);
bool_t							xsemaphore_lock_isr(xsemaphore_t handle);
void							xsemaphore_unlock(xsemaphore_t handle);
void							xsemaphore_unlock_isr(xsemaphore_t handle);

#endif

#endif /* XTASK_H_ */
