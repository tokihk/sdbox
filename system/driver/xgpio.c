/*
 * xgpio.c
 *
 *  Created on: 2024/04/18
 *      Author: Hitoshi Kono
 */

#include "xgpio.h"

#define XGPIO_GATE_PIN_LIST_BEGIN()		XGPIO_PIN_LIST_BEGIN(XGPIO_PIN_LIST)


/* ----------------------------------------------
 * Import board module
 * ------------------------------------------- */
#if XBOARD_GPIO_ENABLE
  #include "driver/xgpio_board.h"

  #define XBOARD_GPIO_NUM		(XCOUNTOF(XGPIO_PIN_LIST))

#else
  #define XBOARD_GPIO_NUM		(0)

#endif


#if XBOARD_GPIO_ENABLE > 0
static struct
{
	bool_t						values[XBOARD_GPIO_NUM];
} g_xgpio;
#endif


void xgpio_init(void)
{
#if XBOARD_GPIO_ENABLE
	xgpio_init_board();
#endif
}

void xgpio_deinit(void)
{
#if XBOARD_GPIO_ENABLE
	xgpio_deinit_board();
#endif
}

uint16_t xgpio_device_num_get(void)
{
	return (XBOARD_GPIO_NUM);
}

bool_t xgpio_open(uint16_t id)
{
	bool_t open_ok = FALSE;

#if XBOARD_GPIO_ENABLE
	if (id < XBOARD_GPIO_NUM)
	{
		open_ok = (XGPIO_PIN_LIST[id].api_open)(&XGPIO_PIN_LIST[id]);
	}
#endif

	return (open_ok);
}

void xgpio_close(uint16_t id)
{
#if XBOARD_GPIO_ENABLE
	if (id < XBOARD_GPIO_NUM)
	{
		(XGPIO_PIN_LIST[id].api_close)(&XGPIO_PIN_LIST[id]);
	}
#endif
}

bool_t xgpio_value_get(uint16_t id)
{
#if XBOARD_GPIO_ENABLE

	bool_t active = FALSE;

	if (id < XBOARD_GPIO_NUM)
	{
		register const xgpio_pin_t *pin = &XGPIO_PIN_LIST[id];

		active = ((pin->api_logic_get)(pin) == pin->logic_active);

		/* Backup */
		g_xgpio.values[id] = active;
	}

	return (active);
#else

	return (XGPIO_INACTIVE);

#endif
}

void xgpio_value_set(uint16_t id, bool_t active)
{
#if XBOARD_GPIO_ENABLE
	if (id < XBOARD_GPIO_NUM)
	{
		register const xgpio_pin_t *pin = &XGPIO_PIN_LIST[id];

		(pin->api_logic_set)(pin, active == pin->logic_active);

		/* Backup */
		g_xgpio.values[id] = active;
	}

#endif
}

void xgpio_value_toggle(uint16_t id)
{
#if XBOARD_GPIO_ENABLE
	xgpio_value_set(id, (bool_t)(!g_xgpio.values[id]));
#endif
}

