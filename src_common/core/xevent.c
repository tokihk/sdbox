/*
 * xevent.c
 *
 *  Created on: 2024/05/03
 *      Author: Hitoshi Kono
 */

#include "xevent.h"


#if XBOARD_OS_TYPE == XBOARD_OS_TYPE_BareMetal

#include "core/xevent_bm.h"

#elif XBOARD_OS_TYPE == XBOARD_OS_TYPE_FreeRTOS

#include "core/xevent_freertos.h"

#endif

