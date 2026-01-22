/*
 * xremcon.h
 *
 *  Created on: 2025/01/03
 *      Author: Hitoshi Kono
 */

#ifndef XREMCON_H_
#define XREMCON_H_

#include "core/xstddef.h"


void							xremcon_init(void);
void							xremcon_deinit(void);

size_t							xremcon_terminal_read(uint8_t *buffer, size_t buffer_size);
size_t							xremcon_terminal_write(const uint8_t *data, size_t data_size);

size_t							xremcon_gate_monitor_write(uint8_t ch, const uint8_t *data, size_t data_size);

void							xremcon_signal_out_set(uint8_t ch, bool_t on);
void							xremcon_signal_out_toggle(uint8_t ch);


#endif /* XREMCON_H_ */
