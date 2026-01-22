/*
 * xuart.c
 *
 *  Created on: 2022/12/18
 *      Author: Hitoshi Kono
 */

#include "xuart.h"


/* ----------------------------------------------
 * Macros for defining device lists
 * ------------------------------------------- */
struct xuart_device_info
{
	bool_t						(*api_open)(const xuart_config_t *config);
	void						(*api_close)(void);
	size_t						(*api_read)(uint8_t *buffer, size_t buffer_size);
	size_t						(*api_write)(const uint8_t *data, size_t data_size);
};

#define XUART_DEVICE_LIST_BEGIN()		static const struct xuart_device_info XUART_DEVICE_LIST[] = {
#define XUART_DEVICE_LIST_END()			};
#define XUART_DEVICE_LIST_ITEM(prefix)	\
{										\
	.api_open=prefix##_open,			\
	.api_close=prefix##_close,			\
	.api_read=prefix##_read,			\
	.api_write=prefix##_write,			\
}

#if XBOARD_UART_ENABLE
  #include "driver/xuart_board.h"

  #define XBOARD_UART_NUM			(XCOUNTOF(XUART_DEVICE_LIST))

#else
  #define XBOARD_UART_NUM			(0)

#endif


void xuart_init(void)
{
#if XBOARD_UART_ENABLE
	xuart_init_board();
#endif
}

void xuart_deinit(void)
{
#if XBOARD_UART_ENABLE
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

#if XBOARD_UART_ENABLE
	if (id < XBOARD_UART_NUM)
	{
		open_ok = (XUART_DEVICE_LIST[id].api_open)(config);
	}
#endif

	return (open_ok);
}

void xuart_close(uint16_t id)
{
#if XBOARD_UART_ENABLE
	if (id < XBOARD_UART_NUM)
	{
		(XUART_DEVICE_LIST[id].api_close)();
	}
#endif
}

size_t xuart_read(uint16_t id, uint8_t *buffer, size_t buffer_size)
{
	size_t read_size = 0;

#if XBOARD_UART_ENABLE
	if (id < XBOARD_UART_NUM)
	{
		read_size = (XUART_DEVICE_LIST[id].api_read)(buffer, buffer_size);
	}
#endif

	return (read_size);
}

size_t xuart_write(uint16_t id, const uint8_t *data, size_t data_size)
{
	size_t write_size = 0;

#if XBOARD_UART_ENABLE
	if (id < XBOARD_UART_NUM)
	{
		write_size = (XUART_DEVICE_LIST[id].api_write)(data, data_size);
	}
#endif

	return (write_size);
}

void xuart_write_sync(uint16_t id, const uint8_t *data, size_t data_size)
{
#if XBOARD_UART_ENABLE
	if (id < XBOARD_UART_NUM)
	{
		size_t write_size = 0;

		while (write_size < data_size)
		{
			write_size += (XUART_DEVICE_LIST[id].api_write)(data + write_size, data_size - write_size);
		}
	}
#endif
}

