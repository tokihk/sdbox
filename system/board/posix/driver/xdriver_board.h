/*
 * xdriver_board.h
 *
 *  Created on: 2024/03/19
 *      Author: Hitoshi Kono
 */

#ifndef XDRIVER_BOARD_H_
#define XDRIVER_BOARD_H_

#include "core/xstddef.h"

#include <time.h>


static struct
{
} g_xdriver_board;


static inline void xdriver_init_board(void)
{
}

static inline void xdriver_wait_us_board(uint32_t time_us)
{
	struct timespec wait_value;

	wait_value.tv_sec = time_us / 1000000;
	wait_value.tv_nsec = (time_us % 1000000) * 1000;

	nanosleep(&wait_value, NULL);
}

#endif /* XDRIVER_BOARD_H_ */
