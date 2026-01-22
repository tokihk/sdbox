/*
 * program.c
 *
 *  Created on: 2024/03/12
 *      Author: Hitoshi Kono
 */

#include <config/xconfig_const.h>
#include "xprogram.h"

#include "core/xtask.h"

#include "driver/xdriver.h"
#include "driver/xgpio.h"
#include "driver/xaudio.h"
#include "driver/xethernet.h"
#include "driver/xinternet.h"
#include "driver/xnetif.h"
#include "driver/xremcon.h"

#include "gate/xgate.h"

#include "general/xwavegen.h"

#include "prompt/xprompt.h"


enum xprogram_task_priority
{
	/* +++ High Priority +++ */

	XPROGRAM_TASKPRIO_10MSEC,
	XPROGRAM_TASKPRIO_100MSEC,
	XPROGRAM_TASKPRIO_1000MSEC,

	XPROGRAM_TASKPRIO_PROMPT,

	/* --- One Shot --- */
	XPROGRAM_TASKPRIO_STARTUP,

	/* --- IDLE task --- */
	XPROGRAM_TASKPRIO_IDLE,

	/* +++ Low Priority +++ */
};


#define XPROGRAM_STARTUP_TASK_PRIO			(XPROGRAM_TASKPRIO_STARTUP)
#define XPROGRAM_STARTUP_TASK_STACK_SIZE	(2048)
#define XPROGRAM_STARTUP_TASK_NAME			(const char *)("startup")

#define XPROGRAM_10MSEC_TASK_PRIO			(XPROGRAM_TASKPRIO_10MSEC)
#define XPROGRAM_10MSEC_TASK_STACK_SIZE		(2048)
#define XPROGRAM_10MSEC_TASK_NAME			(const char *)("10msec")

#define XPROGRAM_100MSEC_TASK_PRIO			(XPROGRAM_TASKPRIO_100MSEC)
#define XPROGRAM_100MSEC_TASK_STACK_SIZE	(2048)
#define XPROGRAM_100MSEC_TASK_NAME			(const char *)("100msec")

#define XPROGRAM_1000MSEC_TASK_PRIO			(XPROGRAM_TASKPRIO_1000MSEC)
#define XPROGRAM_1000MSEC_TASK_STACK_SIZE	(2048)
#define XPROGRAM_1000MSEC_TASK_NAME			(const char *)("1000msec")

#define XPROGRAM_PROMPT_TASK_PRIO			(XPROGRAM_TASKPRIO_PROMPT)
#define XPROGRAM_PROMPT_TASK_STACK_SIZE		(2048)
#define XPROGRAM_PROMPT_TASK_NAME			(const char *)("prompt")

#define XPROGRAM_IDLE_TASK_PRIO				(XPROGRAM_TASKPRIO_IDLE)
#define XPROGRAM_IDLE_TASK_STACK_SIZE		(2048)
#define XPROGRAM_IDLE_TASK_NAME				(const char *)("idle")


struct
{
	xtask_t						task_10msec;
	xtask_t						task_100msec;
	xtask_t						task_1000msec;
	xtask_t						task_prompt;
	xtask_t						task_idle;
} g_xprogram;


static void xprogram_task_10msec(xtask_t handle, void *param1, void *param2)
{
	uint32_t task_tick;

	task_tick = xtask_tick_get();

	for (;;)
	{
		xtask_delay_until(&task_tick, 10);
	}
}

static void xprogram_task_100msec(xtask_t handle, void *param1, void *param2)
{
	uint32_t task_tick;

	task_tick = xtask_tick_get();

	for (;;)
	{
		xtask_delay_until(&task_tick, 100);
	}
}

static void xprogram_task_1000msec(xtask_t handle, void *param1, void *param2)
{
	uint32_t task_tick;

	task_tick = xtask_tick_get();

	for (;;)
	{
		xtask_delay_until(&task_tick, 1000);
	}
}

static void xprogram_task_prompt(xtask_t handle, void *param1, void *param2)
{
	for (;;)
	{
		xprompt_poll();

		xtask_delay(10);
	}
}

static void xprogram_task_idle(xtask_t handle, void *param1, void *param2)
{
	for (;;)
	{
		xgate_poll();
		xnetif_poll();
	}
}

static void xprogram_system_task_startup(void)
{
	g_xprogram.task_10msec = xtask_new(
			xprogram_task_10msec,
			XPROGRAM_10MSEC_TASK_STACK_SIZE,
			XPROGRAM_10MSEC_TASK_PRIO,
			XPROGRAM_10MSEC_TASK_NAME,
			NULL,
			NULL);

	g_xprogram.task_100msec = xtask_new(
			xprogram_task_100msec,
			XPROGRAM_100MSEC_TASK_STACK_SIZE,
			XPROGRAM_100MSEC_TASK_PRIO,
			XPROGRAM_100MSEC_TASK_NAME,
			NULL,
			NULL);

	g_xprogram.task_1000msec = xtask_new(
			xprogram_task_1000msec,
			XPROGRAM_1000MSEC_TASK_STACK_SIZE,
			XPROGRAM_1000MSEC_TASK_PRIO,
			XPROGRAM_1000MSEC_TASK_NAME,
			NULL,
			NULL);

	g_xprogram.task_idle = xtask_new(
			xprogram_task_idle,
			XPROGRAM_IDLE_TASK_STACK_SIZE,
			XPROGRAM_IDLE_TASK_PRIO,
			XPROGRAM_IDLE_TASK_NAME,
			NULL,
			NULL);

	g_xprogram.task_prompt = xtask_new(
			xprogram_task_prompt,
			XPROGRAM_PROMPT_TASK_STACK_SIZE,
			XPROGRAM_PROMPT_TASK_PRIO,
			XPROGRAM_PROMPT_TASK_NAME,
			NULL,
			NULL);
}

static void xprogram_startup_task(xtask_t handle, void *param1, void *param2)
{
	/* Driver initialize */
	xremcon_init();
	xgpio_init();
	xether_init();
	xaudio_init();

	/* Module initialize */
	xprompt_init();
	xgate_init();
	xnetif_init();

	/* システムタスク開始 */
	xprogram_system_task_startup();

	/* --- 起動コマンド --- */

	/* for Debug */
//	xcommand_exec(XPROMPT_ATTR_SILENT, "netif start 0 ipaddr=192.168.100.100");
//	xcommand_exec(XPROMPT_ATTR_SILENT, "netif start 0 ipaddr=127.0.0.1");

//	xcommand_exec(XPROMPT_ATTR_SILENT, "gate setup 0 class=udp remote_addr=192.168.100.200 remote_port=50000 local_port=50000");
//	xcommand_exec(XPROMPT_ATTR_SILENT, "gate setup 0 class=udp remote_addr=127.0.0.1 remote_port=20001 local_port=20000");
//	xcommand_exec(XPROMPT_ATTR_SILENT, "gate setup 1 class=udp remote_addr=192.168.100.200 remote_port=50001 local_port=50001");
//	xcommand_exec(XPROMPT_ATTR_SILENT, "gate setup 4 class=wave-gen format=s16le");
//	xcommand_exec(XPROMPT_ATTR_SILENT, "gate setup 4 class=wave-gen format=u16be");
	xcommand_exec(XPROMPT_ATTR_SILENT, "gate setup 5 class=speaker buffer_size=1600");
//	xcommand_exec(XPROMPT_ATTR_SILENT, "gate setup 5 class=speaker buffer_size=6400");

	xcommand_exec(XPROMPT_ATTR_SILENT, "gate forwarding 4 b100001");
//	xcommand_exec(XPROMPT_ATTR_SILENT, "gate forwarding 0 b100000");

//	xcommand_exec(XPROMPT_ATTR_SILENT, "gate open 0");
	xcommand_exec(XPROMPT_ATTR_SILENT, "gate open 4");
	xcommand_exec(XPROMPT_ATTR_SILENT, "gate open 5");

//	xcommand_exec(XPROMPT_ATTR_SILENT, "gate monitor set 0 4 recv");

#if 0
	/* for Debug */
	ether_conf.mac_addr.addr[0] = 0x00;
	ether_conf.mac_addr.addr[1] = 0x80;
	ether_conf.mac_addr.addr[2] = 0xE1;
	ether_conf.mac_addr.addr[3] = 0x00;
	ether_conf.mac_addr.addr[4] = 0x00;
	ether_conf.mac_addr.addr[5] = 0x00;

	IP4_ADDR(&(inet_iface_conf.ipv4_addr),    192, 168, 100, 100);
	IP4_ADDR(&(inet_iface_conf.ipv4_netmask), 255, 255, 255,   0);
	IP4_ADDR(&(inet_iface_conf.ipv4_gateway), 192, 168, 100,   1);
	inet_iface_conf.mtu = 1500;

	IP_ADDR4(&(inet_udp_conf.local_addr), 192, 168, 100, 100);
	inet_udp_conf.local_port = 50000;
	inet_udp_conf.recv_func = xprogram_udp_recv;

	audio_conf.volume = 150;

	wave_conf.wave_freq = 2000;
	wave_conf.sampling_freq = 16000;
	wave_conf.byte_per_sample = 2;

	xether_open(0, &ether_conf);

	xinet_iface_open(0, &inet_iface_conf);

	xinet_udp_open(&inet_udp_conf);

//	xaudio_open(0, &audio_conf);

	xwave_generate(debug_buff, sizeof(debug_buff), XWAVE_FORM_SINE, &wave_conf);

	for (;;)
	{
		xprompt_poll();

		/* for Debug */
		xether_link_status_update(0);
		xinet_iface_link_update(0);


		pkt = xether_recv_packet_get(0);
		if (pkt != NULL)
		{
			xinet_iface_recv(0, pkt);

			xether_send_packet_set(0, pkt);
		}

//		debug_output_ret = xaudio_pcm_data_add(0, &debug_buff[debug_output_offset], sizeof(debug_buff) - debug_output_offset);
//		debug_output_offset += debug_output_ret;
//		debug_output_offset %= sizeof(debug_buff);
	}

#endif
}

void xprogram_main(void)
{
	xdriver_init();

	/* for Debug */
//	xremcon_init();

	xtask_init(
			xprogram_startup_task,
			XPROGRAM_STARTUP_TASK_STACK_SIZE,
			XPROGRAM_STARTUP_TASK_PRIO,
			XPROGRAM_STARTUP_TASK_NAME,
			NULL,
			NULL);
}

void xprogram_event_set(enum xprogram_event_mask evmask)
{

}

void xprogram_event_clear(enum xprogram_event_mask evmask)
{

}

void xprogram_event_wait(enum xprogram_event_mask evmask)
{

}
