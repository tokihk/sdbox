/*
 * xtask_freertos.h
 *
 *  Created on: 2024/03/31
 *      Author: Hitoshi Kono
 */

#ifndef XTASK_FREERTOS_H_
#define XTASK_FREERTOS_H_

#include "xmalloc.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"


#define XTASK_PRIORITY_MAX			(configMAX_PRIORITIES - 1)


struct xtask_context
{
	TaskHandle_t		handle;
	xtask_func_t		func;

	void *				user_param1;
	void *				user_param2;
};


void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName)
{
	for (;;) {}
}

static void xtask_task_func_base(void *arg)
{
	struct xtask_context *context = (struct xtask_context *)arg;

	if (context->func != NULL)
	{
		(context->func)((xtask_t)context, context->user_param1, context->user_param2);
	}

	/* これ以降、コンテキストへのアクセス禁止 */
	xtask_delete((xtask_t)context);

	for (;;) {}
}

void xtask_init(xtask_func_t main_task, uint32_t stack_size, uint16_t priority, const char *name, void *param1, void *param2)
{
	xtask_new(main_task, stack_size, priority, name, param1, param2);

	vTaskStartScheduler();
}

uint32_t xtask_tick_get(void)
{
	return (xTaskGetTickCount());
}

uint32_t xtask_tick_erapsed_msec_get(uint32_t prev_tick, uint32_t curr_tick)
{
	uint32_t diff_tick;

	if (prev_tick <= curr_tick)
	{
		diff_tick = curr_tick - prev_tick;
	}
	else
	{
		diff_tick = (~(uint32_t)0) - prev_tick + curr_tick + 1;
	}

	return (diff_tick * (1000 / configTICK_RATE_HZ));
}

void xtask_lock(void)
{
	taskENTER_CRITICAL();
}

void xtask_unlock(void)
{
	taskEXIT_CRITICAL();
}

xtask_t xtask_new(xtask_func_t func, uint32_t stack_size, uint16_t priority, const char *name, void *param1, void *param2)
{
	struct xtask_context *context;

	context = xcalloc(sizeof(*context), 1);
	if (context != NULL)
	{
		BaseType_t ret;

		context->func = func;
		context->user_param1 = param1;
		context->user_param2 = param2;

		/* FreeRTOSの優先順位に変換 */
		priority = XMIN(XTASK_PRIORITY_MAX, priority);
		priority = XTASK_PRIORITY_MAX - priority;

		ret = xTaskCreate(
				xtask_task_func_base,
				(name != NULL) ? (name) : (""),
				stack_size / sizeof(StackType_t),
				context,
				priority,
				&(context->handle));

		if (ret != pdPASS)
		{
			xfree(context);
			context = NULL;
		}
	}

	return ((xtask_t)context);
}

xtask_t xtask_new_config(xtask_func_t func, const xtask_config_t *config, void *param1, void *param2)
{
	xtask_t handle = NULL;

	if (config != NULL)
	{
		handle = xtask_new(func, config->stack_size, config->prio, config->name, param1, param2);
	}

	return (handle);
}

void xtask_delete(xtask_t handle)
{
	if (handle != NULL)
	{
		struct xtask_context *context = (struct xtask_context *)handle;

		vTaskDelete(context->handle);
		xfree(context);
	}
}

xtask_t xtask_current_get(void)
{
	xtask_t			handle = NULL;
	TaskHandle_t	handle_raw = xTaskGetCurrentTaskHandle();

	if (handle_raw != NULL)
	{
		handle = (xtask_t)((uint8_t *)handle_raw - XOFFSETOF(struct xtask_context, handle));
	}

	return (handle);
}

void xtask_delay(uint32_t wait_msec)
{
	vTaskDelay(pdMS_TO_TICKS(wait_msec));
}

void xtask_delay_until(uint32_t *curr_tick, uint32_t wait_msec)
{
	if ((curr_tick != NULL) && (wait_msec > 0))
	{
		TickType_t tick = pdMS_TO_TICKS(wait_msec);

		xTaskDelayUntil(curr_tick, XMAX(tick, 1));
	}
}

uint32_t xtask_delay_minimum_get(void)
{
	return ((uint32_t)1000 / configTICK_RATE_HZ);
}

xmutex_t xmutex_new(void)
{
	SemaphoreHandle_t handle;

	handle = xSemaphoreCreateMutex();
	if (handle != NULL) {
		vQueueAddToRegistry(handle, NULL);
	}

	return ((xmutex_t)handle);
}

void xmutex_delete(xmutex_t handle)
{
	if (handle != NULL)
	{
		vQueueUnregisterQueue((SemaphoreHandle_t)handle);

		vSemaphoreDelete((SemaphoreHandle_t)handle);
	}
}

bool_t xmutex_lock(xmutex_t handle, uint32_t timeout_msec)
{
	bool_t success = FALSE;

	if (handle != NULL)
	{
		TickType_t timeout_tick;

		if (timeout_msec == XTIMEOUT_INFINITE)
		{
			timeout_tick = portMAX_DELAY;
		}
		else
		{
			timeout_tick = pdMS_TO_TICKS(timeout_msec);
		}

		if (xSemaphoreTake((SemaphoreHandle_t)handle, timeout_tick) == pdPASS)
		{
			success = TRUE;
		}
	}

	return (success);
}

bool_t xmutex_lock_isr(xmutex_t handle)
{
	bool_t success = FALSE;

	if (handle != NULL)
	{
		BaseType_t yield;

		yield = pdFALSE;
		if (xSemaphoreTakeFromISR((SemaphoreHandle_t)handle, &yield) == pdPASS)
		{
			success = TRUE;
			portYIELD_FROM_ISR(yield);
		}
	}

	return (success);
}

void xmutex_unlock(xmutex_t handle)
{
	if (handle != NULL)
	{
		xSemaphoreGive((SemaphoreHandle_t)handle);
	}
}

void xmutex_unlock_isr(xmutex_t handle)
{
	if (handle != NULL)
	{
		BaseType_t yield;

		yield = pdFALSE;
		if (xSemaphoreGiveFromISR((SemaphoreHandle_t)handle, &yield) == pdTRUE)
		{
			portYIELD_FROM_ISR(yield);
		}
	}
}

xsemaphore_t xsemaphore_new(uint32_t max_count, uint32_t initial_count)
{
	SemaphoreHandle_t handle = NULL;

	if ((max_count > 0) && (initial_count <= max_count))
	{
		handle = xSemaphoreCreateCounting(max_count, initial_count);

#if (configQUEUE_REGISTRY_SIZE > 0)
		vQueueAddToRegistry(handle, NULL);
#endif
	}

	return ((xsemaphore_t)handle);
}

void xsemaphore_delete(xsemaphore_t handle)
{
	if (handle != NULL)
	{
#if (configQUEUE_REGISTRY_SIZE > 0)
		vQueueUnregisterQueue((SemaphoreHandle_t)handle);
#endif

		vSemaphoreDelete((SemaphoreHandle_t)handle);
	}
}

uint32_t xsemaphore_count_get(xsemaphore_t handle)
{
	uint32_t count = 0;

	if (handle != NULL)
	{
		count = (uint32_t)uxSemaphoreGetCount((SemaphoreHandle_t)handle);
	}

	return (count);
}

uint32_t xsemaphore_count_get_isr(xsemaphore_t handle)
{
	uint32_t count = 0;

	if (handle != NULL)
	{
		count = uxQueueMessagesWaitingFromISR((SemaphoreHandle_t)handle);
	}

	return (count);
}

bool_t xsemaphore_lock(xsemaphore_t handle, uint32_t timeout_msec)
{
	bool_t success = FALSE;

	if (handle != NULL)
	{
		TickType_t timeout_tick;

		if (timeout_msec == XTIMEOUT_INFINITE)
		{
			timeout_tick = portMAX_DELAY;
		}
		else
		{
			timeout_tick = pdMS_TO_TICKS(timeout_msec);
		}

		if (xSemaphoreTake((SemaphoreHandle_t)handle, timeout_tick) == pdPASS)
		{
			success = TRUE;
		}
	}

	return (success);
}

bool_t xsemaphore_lock_isr(xsemaphore_t handle)
{
	bool_t success = FALSE;

	if (handle != NULL)
	{
		BaseType_t yield;

		yield = pdFALSE;
		if (xSemaphoreTakeFromISR((SemaphoreHandle_t)handle, &yield) == pdPASS)
		{
			success = TRUE;
			portYIELD_FROM_ISR (yield);
		}
	}

	return (success);
}

void xsemaphore_unlock(xsemaphore_t handle)
{
	if (handle != NULL)
	{
		xSemaphoreGive((SemaphoreHandle_t)handle);
	}
}

void xsemaphore_unlock_isr(xsemaphore_t handle)
{
	if (handle != NULL)
	{
		BaseType_t yield;

		yield = pdFALSE;
		if (xSemaphoreGiveFromISR((SemaphoreHandle_t)handle, &yield) == pdTRUE)
		{
			portYIELD_FROM_ISR(yield);
		}
	}
}


#endif /* XTASK_FREERTOS_H_ */
