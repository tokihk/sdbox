/*
 * xgpio_board.h
 *
 *  Created on: 2024/12/16
 *      Author: Hitoshi Kono
 */

#ifndef XGPIO_BOARD_H_
#define XGPIO_BOARD_H_


static void xgpio_init_board(void)
{
}

static inline void xgpio_deinit_board(void)
{
}

static inline bool_t xgpio_pin_logic_get(const struct xgpio_pin_info *pin_info)
{
	return (FALSE);
}

static inline void xgpio_pin_logic_set(const struct xgpio_pin_info *pin_info, bool_t logic)
{
}

XGPIO_GATE_PIN_LIST_BEGIN()

XGPIO_PIN_LIST_END()

#endif /* XGPIO_BOARD_H_ */
