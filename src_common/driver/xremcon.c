/*
 * xremcon.c
 *
 *  Created on: 2025/01/03
 *      Author: Hitoshi Kono
 */

#include "xremcon.h"

#include "driver/xremcon_board.h"


void xremcon_init(void)
{
	xremcon_init_board();
}

void xremcon_deinit(void)
{
	xremcon_deinit_board();
}

size_t xremcon_terminal_read(uint8_t *buffer, size_t buffer_size)
{
	size_t read_size = 0;

	if ((buffer != NULL) && (buffer_size > 0))
	{
		read_size = xremcon_terminal_read_board(buffer, buffer_size);
	}

	return (read_size);
}

size_t xremcon_terminal_write(const uint8_t *data, size_t data_size)
{
	size_t write_size = 0;

	if ((data != NULL) && (data_size > 0))
	{
		write_size = xremcon_terminal_write_board(data, data_size);
	}

	return (write_size);
}

size_t xremcon_gate_monitor_write(uint8_t ch, const uint8_t *data, size_t data_size)
{
	size_t write_size = 0;

	if ((data != NULL) && (data_size > 0))
	{
		write_size = xremcon_gate_monitor_write_board(ch, data, data_size);
	}

	return (write_size);
}

void xremcon_signal_out_set(uint8_t ch, bool_t on)
{
	xremcon_signal_out_set_board(ch, on);
}

void xremcon_signal_out_toggle(uint8_t ch)
{
	xremcon_signal_out_toggle_board(ch);
}
