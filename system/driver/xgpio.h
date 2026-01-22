/*
 * xgpio.h
 *
 *  Created on: 2024/04/18
 *      Author: Hitoshi Kono
 */

#ifndef XGPIO_H_
#define XGPIO_H_

#include "core/xstddef.h"


typedef struct xgpio_pin xgpio_pin_t;


#define XGPIO_PIN_LIST_BEGIN(name)		static const struct xgpio_pin name[] = {
#define XGPIO_PIN_LIST_END()			};
#define XGPIO_PIN_LIST_ITEM(prefix, dir_, logic_act_, logic_init_, ...)		\
{											\
	.dir=dir_,								\
	.logic_active=logic_act_,				\
	.logic_init=logic_init_,				\
	.params={__VA_ARGS__},					\
	.api_logic_get=prefix##_logic_get,		\
	.api_logic_set=prefix##_logic_set,		\
}


enum xgpio_direction
{
	XGPIP_DIR_INPUT,
	XGPIO_DIR_OUTPUT,
};

struct xgpio_pin
{
	enum xgpio_direction		dir;
	bool_t						logic_active;
	bool_t						logic_init;			/* for output port */
	uint32_t					params[4];

	bool_t						(*api_open)(const xgpio_pin_t *pin);
	void						(*api_close)(const xgpio_pin_t *pin);
	bool_t						(*api_logic_get)(const xgpio_pin_t *pin);
	void						(*api_logic_set)(const xgpio_pin_t *pin, bool_t logic);
};


void							xgpio_init(void);
void							xgpio_deinit(void);

uint16_t						xgpio_device_num_get(void);

bool_t							xgpio_open(uint16_t id);
void							xgpio_close(uint16_t id);

bool_t							xgpio_value_get(uint16_t id);

void							xgpio_value_set(uint16_t id, bool_t active);

void							xgpio_value_toggle(uint16_t id);


#endif /* XGPIO_H_ */
