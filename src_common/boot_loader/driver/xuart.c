/*
 * xuart.c
 *
 *  Created on: 2022/12/18
 *      Author: Hitoshi Kono
 */

#include "xuart.h"

#if XBOARD_UART_ENABLE
#include "driver/xuart_board.h"
#else
#define XBOARD_UART_NUM			(0)
#endif


void xuart_init(void)
{
#if XBOARD_UART_NUM > 0
	xuart_init_board();
#endif
}

void xuart_deinit(void)
{
#if XBOARD_UART_NUM > 0
	xuart_deinit_board();
#endif
}

uint16_t xuart_device_num_get(void)
{
	return (XBOARD_UART_NUM);
}

bool_t xuart_open(uint16_t id, const xuart_config_t *config)
{
	bool_t open_ok = FALSE;

#if XBOARD_UART_NUM > 0
	if (id < XBOARD_UART_NUM) {
		open_ok = xuart_open_board(id, config);
	}
#endif

	return (open_ok);
}

void xuart_close(uint16_t id)
{
#if XBOARD_UART_NUM > 0
	if (id < XBOARD_UART_NUM) {
		xuart_close_board(id);
	}
#endif
}

size_t xuart_read(uint16_t id, uint8_t *buffer, size_t buffer_size)
{
	size_t read_size = 0;

#if XBOARD_UART_NUM > 0
	if (id < XBOARD_UART_NUM) {
		read_size = xuart_read_board(id, buffer, buffer_size);
	}
#endif

	return (read_size);
}

size_t xuart_write(uint16_t id, const uint8_t *data, size_t data_size)
{
	size_t write_size = 0;

#if XBOARD_UART_NUM > 0
	if (id < XBOARD_UART_NUM) {
		write_size = xuart_write_board(id, data, data_size);
	}
#endif

	return (write_size);
}

void xuart_write_sync(uint16_t id, const uint8_t *data, size_t data_size)
{
#if XBOARD_UART_NUM > 0
	if (id < XBOARD_UART_NUM) {
		size_t write_size = 0;

		while (write_size < data_size) {
			write_size += xuart_write(id, data + write_size, data_size - write_size);
		}
	}
#endif
}

