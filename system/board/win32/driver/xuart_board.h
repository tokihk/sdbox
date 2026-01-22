/*
 * xuart_board.h
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */

#ifndef XUART_BOARD_H_
#define XUART_BOARD_H_

#include "driver/xuart.h"


static bool_t xuart_win32_open(const xuart_config_t *config)
{
	return (TRUE);
}

static void xuart_win32_close(void)
{
}

static size_t xuart_win32_read(uint8_t *buffer, size_t buffer_size)
{
	return (0);
}

static size_t xuart_win32_write(const uint8_t *data, size_t data_size)
{
	return (data_size);
}

static inline void xuart_init_board(void)
{
}

static inline void xuart_deinit_board(void)
{
}

XUART_DEVICE_LIST_BEGIN()
  XUART_DEVICE_LIST_ITEM(xuart_win32),
XUART_DEVICE_LIST_END()

#endif /* XUART_BOARD_H_ */
