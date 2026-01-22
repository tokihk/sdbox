/*
 * xtimer.c
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */

#include "xtimer.h"


xtimer_manager_t xtimer_manager_new(void)
{

}

void xtimer_manager_delete(xtimer_manager_t mgr)
{

}

void xtimer_poll(xtimer_manager_t mgr)
{

}

xtimer_t xtimer_new(xtimer_manager_t mgr, xtimer_func_t func, bool_t reload, const char *name, void *param)
{

}

void xtimer_delete(xtimer_t handle)
{

}

bool_t xtimer_start(xtimer_t handle, uint32_t period_msec)
{

}

void xtimer_stop(xtimer_t handle)
{

}

bool_t xtimer_is_running(xtimer_t handle)
{

}

