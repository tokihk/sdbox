/*
 * xdriver_board.h
 *
 *  Created on: 2024/03/19
 *      Author: Hitoshi Kono
 */

#ifndef XDRIVER_BOARD_H_
#define XDRIVER_BOARD_H_

#include "core/xstddef.h"

#include <Windows.h>
#include <MMsystem.h>


static struct
{
	HANDLE				htimer;
} g_xdriver_board;


static inline void xdriver_init_board(void)
{
	g_xdriver_board.htimer = CreateWaitableTimer(NULL, TRUE, NULL);
}

static inline void xdriver_wait_us_board(uint32_t time_us)
{
#if 1
	LARGE_INTEGER	cpu_freq;
	LARGE_INTEGER	count_start;
	LARGE_INTEGER	count_curr;
	LONGLONG		cpu_freq_us;

	/* 呼び出し時のオーバーヘッドをざっくり補正 */
	if (time_us >= 20)
	{
		time_us -= 20;
	}

	QueryPerformanceFrequency(&cpu_freq);
	cpu_freq_us = cpu_freq.QuadPart / 1000000;

	QueryPerformanceCounter(&count_start);

	/* 必ず1ms以上のSleepを回す */
	if (time_us > 2000)
	{
		Sleep(1);
	}

	/* 残りはpollで微調整 */
	do
	{
		QueryPerformanceCounter(&count_curr);
//		Sleep(0);
	}
	while (((count_curr.QuadPart - count_start.QuadPart) / cpu_freq_us) <= time_us);
#else
	LARGE_INTEGER		wait_time;

	wait_time.QuadPart = (LONGLONG)time_us * (-10);

	if (SetWaitableTimer(g_xdriver_board.htimer, &wait_time, 0, NULL, NULL, FALSE))
	{
		WaitForSingleObject(g_xdriver_board.htimer, INFINITE);
	}
#endif
}

#endif /* XDRIVER_BOARD_H_ */
