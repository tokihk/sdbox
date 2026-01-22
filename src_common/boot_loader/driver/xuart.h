/*
 * xuart.h
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */

#ifndef XUART_H_
#define XUART_H_

#include "driver/xboard.h"


enum xuart_parity
{
	XUART_PARITY_NONE,
	XUART_PARITY_ODD,
	XUART_PARITY_EVEN,
};

enum xuart_stopbit
{
	XUART_STOPBIT_ONE,
	XUART_STOPBIT_TWO,
};

typedef struct xuart_config
{
	uint32_t					baudrate;
	uint8_t						data_bits;
	enum xuart_parity			parity;
	enum xuart_stopbit			stop_bits;
} xuart_config_t;


void							xuart_init(void);
void							xuart_shutdown(void);

uint16_t						xuart_device_num_get(void);

bool_t							xuart_open(uint16_t id, const xuart_config_t *config);
void							xuart_close(uint16_t id);

size_t							xuart_read(uint16_t id, uint8_t *buffer, size_t buffer_size);
size_t							xuart_write(uint16_t id, const uint8_t *data, size_t data_size);

#endif /* XUART_H_ */
