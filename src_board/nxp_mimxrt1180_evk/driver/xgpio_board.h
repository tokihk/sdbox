/*
 * xgpio_board.h
 *
 *  Created on: 2024/04/18
 *      Author: Hitoshi Kono
 */

#ifndef XGPIO_BOARD_H_
#define XGPIO_BOARD_H_

#include "pin_mux.h"
#include "board.h"


static void xgpio_init_board(void)
{
}

static inline void xgpio_deinit_board(void)
{
}

/*
 * params[0] = (GPIO_Type *)port
 * params[1] = (uint32_t)pin
 */

bool_t xgpio_pin_open(const xgpio_pin_t *pin)
{
	gpio_pin_config_t pin_config;

	pin_config.direction = (pin->dir == XGPIO_DIR_OUTPUT) ? (kGPIO_DigitalOutput) : (kGPIO_DigitalInput);
	pin_config.outputLogic = pin->logic_init;
	pin_config.interruptMode = (gpio_interrupt_mode_t)pin->params[2];

	GPIO_PinInit((GPIO_Type *)pin->params[0], (uint32_t)pin->params[2], &pin_config);

	return (TRUE);
}

void xgpio_pin_close(const xgpio_pin_t *pin)
{
}

/*
 * params[0] = (GPIO_Type *)port
 * params[1] = (uint32_t)pin
 */

bool_t xgpio_pin_logic_get(const xgpio_pin_t *pin)
{
	return ((((GPIO_Type *)(pin->params[0]))->DR >> (uint32_t)(pin->params[1])) & 0x1);
}

/*
 * params[0] = (GPIO_Type *)port
 * params[1] = (uint32_t)pin
 */
void xgpio_pin_logic_set(const xgpio_pin_t *pin, bool_t logic)
{
	if (logic)
	{
		((GPIO_Type *)(pin->params[0]))->DR_SET = 1u << (uint32_t)(pin->params[1]);
	}
	else
	{
		((GPIO_Type *)(pin->params[0]))->DR_CLEAR = 1u << (uint32_t)(pin->params[1]);
	}
}

void xgpio_pin_logic_toggle(const xgpio_pin_t *pin)
{
	((GPIO_Type *)(pin->params[0]))->DR_TOGGLE = 1u << (uint32_t)(pin->params[1]);
}


XGPIO_GATE_PIN_LIST_BEGIN()

XGPIO_PIN_LIST_END()

#if 0
{
	.port         = GPIO9,
	.pin          = 3,
	.active_value = 1,
	.config = {
		.direction     = kGPIO_DigitalOutput,
		.outputLogic   = 0,
		.interruptMode = kGPIO_NoIntmode
	}
},

/* USER LED 2 */
{
	.port         = GPIO9,
	.pin          = 25,
	.active_value = 1,
	.config = {
		.direction     = kGPIO_DigitalOutput,
		.outputLogic   = 0,
		.interruptMode = kGPIO_NoIntmode
	}
},

/* J25-15 */
{
	.port         = GPIO8,
	.pin          = 31,
	.active_value = 1,
	.config = {
		.direction     = kGPIO_DigitalOutput,
		.outputLogic   = 0,
		.interruptMode = kGPIO_NoIntmode
	}
},

/* J25-13 */
{
	.port         = GPIO9,
	.pin          = 0,
	.active_value = 1,
	.config = {
		.direction     = kGPIO_DigitalOutput,
		.outputLogic   = 0,
		.interruptMode = kGPIO_NoIntmode
	}
},

/* J25-11 */
{
	.port         = GPIO9,
	.pin          = 1,
	.active_value = 1,
	.config = {
		.direction     = kGPIO_DigitalOutput,
		.outputLogic   = 0,
		.interruptMode = kGPIO_NoIntmode
	}
},

/* J25-9 */
{
	.port         = GPIO9,
	.pin          = 2,
	.active_value = 1,
	.config = {
		.direction     = kGPIO_DigitalOutput,
		.outputLogic   = 0,
		.interruptMode = kGPIO_NoIntmode
	}
},

/* J25-7 */
{
	.port         = GPIO9,
	.pin          = 3,
	.active_value = 1,
	.config = {
		.direction     = kGPIO_DigitalOutput,
		.outputLogic   = 0,
		.interruptMode = kGPIO_NoIntmode
	}
},
#endif

#endif /* XGPIO_BOARD_H_ */
