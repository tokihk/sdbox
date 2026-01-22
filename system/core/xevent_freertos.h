/*
 * xevent_freertos.h
 *
 *  Created on: 2024/05/03
 *      Author: Hitoshi Kono
 */

#ifndef XEVENT_FREERTOS_H_
#define XEVENT_FREERTOS_H_

#include "FreeRTOS.h"
#include "event_groups.h"


xevent_group_t xevent_group_new(void)
{
	return ((xevent_group_t)xEventGroupCreate());
}

void xevent_group_delete(xevent_group_t handle)
{
	if (handle != NULL) {
		vEventGroupDelete((EventGroupHandle_t)handle);
	}
}

uint32_t xevent_group_flags_clear(xevent_group_t handle, uint32_t flags)
{
	uint32_t rflags = 0;

	if (handle != NULL) {
		rflags = xEventGroupClearBits((EventGroupHandle_t)handle, (EventBits_t)flags);
	}

	return (rflags);
}

uint32_t xevent_group_flags_get(xevent_group_t handle)
{
	uint32_t flags = 0;

	if (handle != NULL) {
		flags = xEventGroupGetBits((EventGroupHandle_t)handle);
	}

	return (flags);
}

uint32_t xevent_group_flags_set(xevent_group_t handle, uint32_t flags)
{
	uint32_t rflags = 0;

	if (handle != NULL) {
		rflags = xEventGroupSetBits((EventGroupHandle_t)handle, (EventBits_t)flags);
	}

	return (rflags);
}

uint32_t xevent_group_flags_wait(xevent_group_t handle, uint32_t flags, bool_t flags_clear, bool_t wait_all, uint32_t timeout_msec)
{
	uint32_t rflags = 0;

	if (handle != NULL) {
		TickType_t timeout_tick;

		if (timeout_msec == XTIMEOUT_INFINITE) {
			timeout_tick = portMAX_DELAY;
		} else {
			timeout_tick = pdMS_TO_TICKS(timeout_msec);
		}

		rflags = xEventGroupWaitBits((EventGroupHandle_t)handle, (EventBits_t)flags, flags_clear, wait_all, timeout_tick);
	}

	return (rflags);
}

uint32_t xevent_group_flags_clear_isr(xevent_group_t handle, uint32_t flags)
{
	uint32_t rflags = 0;

	if (handle != NULL) {
		rflags = xEventGroupClearBitsFromISR((EventGroupHandle_t)handle, (EventBits_t)flags);
	}

	return (rflags);
}

uint32_t xevent_group_flags_get_isr(xevent_group_t handle)
{
	uint32_t flags = 0;

	if (handle != NULL) {
		flags = xEventGroupGetBitsFromISR((EventGroupHandle_t)handle);
	}

	return (flags);
}

uint32_t xevent_group_flags_set_isr(xevent_group_t handle, uint32_t flags)
{
	uint32_t rflags = 0;

	if (handle != NULL) {
		BaseType_t yield = pdFALSE;

		if (xEventGroupSetBitsFromISR((EventGroupHandle_t)handle, (EventBits_t)flags, &yield)) {
			rflags = flags;
			portYIELD_FROM_ISR(yield);
		}
	}

	return (rflags);
}


#endif /* XEVENT_FREERTOS_H_ */
