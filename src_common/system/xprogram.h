/*
 * xprogram.h
 *
 *  Created on: 2024/03/12
 *      Author: Hitoshi Kono
 */

#ifndef XPROGRAM_H_
#define XPROGRAM_H_


enum xprogram_task_id
{
	XPROGRAM_TASKID_STARTUP,
	XPROGRAM_TASKID_PROMPT,

	XPROGRAM_TASKID_MAX,
};

enum xprogram_event_mask
{
	XPROGRAM_EVMASK_PROMPT_RUNNING	= 1u << 0,
	XPROGRAM_EVMASK_GATE_RUNNING	= 1u << 1,

	XPROGRAM_EVMASK_SYSTEM_RUNNING	= (XPROGRAM_EVMASK_PROMPT_RUNNING | XPROGRAM_EVMASK_GATE_RUNNING),
};



void							xprogram_main(void);

void							xprogram_event_set(enum xprogram_event_mask evmask);
void							xprogram_event_clear(enum xprogram_event_mask evmask);
void							xprogram_event_wait(enum xprogram_event_mask evmask);


#endif /* XPROGRAM_H_ */
