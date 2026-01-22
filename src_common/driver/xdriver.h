/*
 * xdriver.h
 *
 *  Created on: 2024/03/19
 *      Author: Hitoshi Kono
 */

#ifndef XDRIVER_H_
#define XDRIVER_H_

#include "core/xstddef.h"


#define XTICK_MSEC				(1000u / XBOARD_TICK_BASE_USEC)

#define XTICK_TO_MSEC(tick)		((uint32_t)(tick) / XBOARD_TICK_MSEC)
#define XMSEC_TO_TICK(msec)		((uint32_t)(msec) * XBOARD_TICK_MSEC)


void							xdriver_init(void);
void							xdriver_deinit(void);

uint32_t						xdriver_tick_get(void);

void							xdriver_wait_us(uint32_t time_us);


#endif /* XDRIVER_H_ */
