/*
 * xmalloc.c
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */


#include "xmalloc.h"

#include <string.h>


#if XCORE_CONF_OS_TYPE == XCORE_CONF_OS_FREE_RTOS

#include "FreeRTOS.h"

#define XMALLOC_BASE			pvPortMalloc
#define XFREE_BASE				vPortFree

#else

#define XMALLOC_BASE			malloc
#define XFREE_BASE				free

#endif


void *xmalloc(size_t size)
{
	return (XMALLOC_BASE(size));
}

void xfree(void *ptr)
{
	XFREE_BASE(ptr);
}

void *xcalloc(size_t n, size_t size)
{
	void *ptr;

	ptr = xmalloc(n * size);
	if (ptr != NULL)
	{
		memset(ptr, 0, n * size);
	}

	return (ptr);
}

