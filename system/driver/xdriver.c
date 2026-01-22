/*
 * xdriver.c
 *
 *  Created on: 2024/03/19
 *      Author: Hitoshi Kono
 */

#include "xdriver.h"

#include "driver/xdriver_board.h"


void xdriver_init(void)
{
	xdriver_init_board();
}

void xdriver_deinit(void)
{

}

uint32_t xdriver_tick_get(void)
{
	return (0);
}

void xdriver_wait_us(uint32_t time_us)
{
	xdriver_wait_us_board(time_us);
}
