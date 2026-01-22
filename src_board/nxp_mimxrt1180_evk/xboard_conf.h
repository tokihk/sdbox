/*
 * xboard_conf.h
 *
 *  Created on: 2024/03/31
 *      Author: Hitoshi Kono
 */

#ifndef XBOARD_CONF_H_
#define XBOARD_CONF_H_

#define XBOARD_OS_TYPE_BareMetal	(0)
#define XBOARD_OS_TYPE_FreeRTOS		(1)

#ifndef XBOARD_OS_TYPE
#define XBOARD_OS_TYPE				(XBOARD_OS_TYPE_FreeRTOS)
#endif

#ifndef XBOARD_GPIO_ENABLE
#define XBOARD_GPIO_ENABLE			(1)
#endif

#ifndef XBOARD_UART_ENABLE
#define XBOARD_UART_ENABLE			(1)
#endif

#ifndef XBOARD_ETHERNET_ENABLE
#define XBOARD_ETHERNET_ENABLE		(1)
#endif

#ifndef XBOARD_AUDIO_ENABLE
#define XBOARD_AUDIO_ENABLE			(1)
#endif

#ifndef XBOARD_USB_HOST_ENABLE
#define XBOARD_USB_HOST_ENABLE		(0)
#endif

#ifndef XBOARD_LCD_ENABLE
#define XBOARD_LCD_ENABLE			(0)
#endif

#ifndef XBOARD_TICK_BASE_USEC
#define XBOARD_TICK_BASE_USEC		100u
#endif

#endif /* XBOARD_CONF_H_ */
