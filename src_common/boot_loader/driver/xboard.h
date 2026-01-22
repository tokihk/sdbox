/*
 * xboard.h
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */

#ifndef XBOARD_H_
#define XBOARD_H_

#include "core/xstddef.h"

#include "driver/xboard_config.h"


#define XTICK_MSEC				(1000u / XBOARD_TICK_BASE_USEC)

#define XTICK_TO_MSEC(tick)		((uint32_t)(tick) / XBOARD_TICK_MSEC)
#define XMSEC_TO_TICK(msec)		((uint32_t)(msec) * XBOARD_TICK_MSEC)


void							xboard_init(void);
void							xboard_shutdown(void);

uint32_t						xboard_tick_get(void);

void							xboard_wait_us(uint32_t time_us);


#endif /* XBOARD_H_ */
