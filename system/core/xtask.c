/*
 * xtask.c
 *
 *  Created on: 2024/03/31
 *      Author: Hitoshi Kono
 */

#include "xtask.h"


#if XBOARD_OS_TYPE == XBOARD_OS_TYPE_BareMetal

#include "core/xtask_bm.h"

#elif XBOARD_OS_TYPE == XBOARD_OS_TYPE_FreeRTOS

#include "core/xtask_freertos.h"

#endif
