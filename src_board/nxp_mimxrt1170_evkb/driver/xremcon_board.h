/*
 * xremcon_board.h
 *
 *  Created on: 2025/01/03
 *      Author: Hitoshi Kono
 */

#ifndef XREMCON_BOARD_H_
#define XREMCON_BOARD_H_

#include "driver/xuart.h"
#include "driver/xgpio.h"

#include "pin_mux.h"
#include "board.h"


#define XREMCON_TERMINAL_UART_ID	(0)


static struct
{
} g_xremcon_board;


static const xuart_config_t XREMCON_PROMPT_UART_CONFIG =
{
	.baudrate  = 115200,
	.data_bits = 8,
	.parity    = XUART_PARITY_NONE,
	.stop_bits = XUART_STOPBIT_ONE,
};


bool_t							xuart_lpuart1_open(const xuart_config_t *config);
void							xuart_lpuart1_close(void);
size_t							xuart_lpuart1_read(uint8_t *buffer, size_t buffer_size);
size_t							xuart_lpuart1_write(const uint8_t *data, size_t data_size);

bool_t							xgpio_pin_open(const xgpio_pin_t *pin);
void							xgpio_pin_close(const xgpio_pin_t *pin);
bool_t							xgpio_pin_logic_get(const xgpio_pin_t *pin);
void							xgpio_pin_logic_set(const xgpio_pin_t *pin, bool_t logic);
void							xgpio_pin_logic_toggle(const xgpio_pin_t *pin);


XGPIO_PIN_LIST_BEGIN(XREMCON_SIGNAL_GPIO_PIN)

	/* USER LED 1 */
	XGPIO_PIN_LIST_ITEM(xgpio_pin, XGPIO_DIR_OUTPUT, 1, 0, (uint32_t)GPIO9, 3,  kGPIO_NoIntmode),

	/* USER LED 2 */
	XGPIO_PIN_LIST_ITEM(xgpio_pin, XGPIO_DIR_OUTPUT, 1, 0, (uint32_t)GPIO9, 25, kGPIO_NoIntmode),

	/* J25-15 */
	XGPIO_PIN_LIST_ITEM(xgpio_pin, XGPIO_DIR_OUTPUT, 1, 0, (uint32_t)GPIO8, 31, kGPIO_NoIntmode),

	/* J25-13 */
	XGPIO_PIN_LIST_ITEM(xgpio_pin, XGPIO_DIR_OUTPUT, 1, 0, (uint32_t)GPIO9, 0,  kGPIO_NoIntmode),

	/* J25-11 */
	XGPIO_PIN_LIST_ITEM(xgpio_pin, XGPIO_DIR_OUTPUT, 1, 0, (uint32_t)GPIO9, 1,  kGPIO_NoIntmode),

	/* J25-9 */
	XGPIO_PIN_LIST_ITEM(xgpio_pin, XGPIO_DIR_OUTPUT, 1, 0, (uint32_t)GPIO9, 2,  kGPIO_NoIntmode),

	/* J25-7 */
	XGPIO_PIN_LIST_ITEM(xgpio_pin, XGPIO_DIR_OUTPUT, 1, 0, (uint32_t)GPIO9, 3,  kGPIO_NoIntmode),

XGPIO_PIN_LIST_END()


static void xremcon_signal_gpio_init(void)
{
	uint16_t index;

	for (index = 0; index < XCOUNTOF(XREMCON_SIGNAL_GPIO_PIN); index++)
	{
		xgpio_pin_open(&XREMCON_SIGNAL_GPIO_PIN[index]);
	}
}

static inline void xremcon_init_board(void)
{
	xremcon_signal_gpio_init();

	xuart_lpuart1_open(&XREMCON_PROMPT_UART_CONFIG);
}

static inline void xremcon_deinit_board(void)
{
}

static inline size_t xremcon_terminal_read_board(uint8_t *buffer, size_t buffer_size)
{
	return (xuart_lpuart1_read(buffer, (uint16_t)buffer_size));
}

static inline size_t xremcon_terminal_write_board(const uint8_t *data, size_t data_size)
{
	return (xuart_lpuart1_write(data, data_size));
}

static inline size_t xremcon_gate_monitor_write_board(uint8_t ch, const uint8_t *data, size_t data_size)
{
	return (0);
}

static inline void xremcon_signal_out_set_board(uint8_t ch, bool_t on)
{
	if (ch < XCOUNTOF(XREMCON_SIGNAL_GPIO_PIN))
	{
		xgpio_pin_logic_set(&XREMCON_SIGNAL_GPIO_PIN[ch], on);
	}
}

static inline void xremcon_signal_out_toggle_board(uint8_t ch)
{
	if (ch < XCOUNTOF(XREMCON_SIGNAL_GPIO_PIN))
	{
		xgpio_pin_logic_toggle(&XREMCON_SIGNAL_GPIO_PIN[ch]);
	}
}

#endif /* XREMCON_BOARD_H_ */
