/*
 * xgate.c
 *
 *  Created on: 2024/05/01
 *      Author: Hitoshi Kono
 */

#include <config/xconfig_const.h>
#include "xgate.h"

#include "xgate_speaker.h"
#include "xgate_udp.h"
#include "xgate_wave_gen.h"

#include "core/xevent.h"
#include "core/xtask.h"
#include "core/xmalloc.h"

#include "driver/xremcon.h"

#include "system/xprogram.h"

#include <stdio.h>


#define XGATE_OBJECT_FLOW_MAX			(XGATE_OBJECT_NUM_MAX * 5)

#define XGATE_EVFLAG_OPEN_UPDATE		(1u << 0u)

#define XGATE_XFLOW_CODE_XON_DEFAULT	(0x11)
#define XGATE_XFLOW_CODE_XOFF_DEFAULT	(0x13)

#define XGATE_OBJECT_NO(gobj)			((xgate_object_t *)(gobj) - &g_xgate.obj_list[0])


static const xgate_class_func_t XGATE_CLASS_LIST[] =
{
	xgate_udp_class,
	xgate_wave_gen_class,
	xgate_speaker_class,
};

typedef struct xgate_monitor_object
{

} xgate_monitor_object_t;

static struct
{
	xgate_class_t				class_list[XCOUNTOF(XGATE_CLASS_LIST)];
	xgate_object_t				obj_list[XGATE_OBJECT_NUM_MAX];

	uint32_t					forwarding_to_gate_mask[XGATE_OBJECT_NUM_MAX];		/* 自身が受信したパケットの転送先ゲート一覧     */
	uint32_t					forwarding_from_gate_mask[XGATE_OBJECT_NUM_MAX];	/* 自身にパケットを転送してくる転送元ゲート一覧 */

	xmutex_t					mtx_gate_object;
} g_xgate;


static bool_t xgate_class_config_init(xgate_class_config_t *gclass_conf, xgate_class_t *gclass_new)
{
	bool_t success = FALSE;

	if (   (gclass_conf != NULL)
		&& (gclass_new != NULL)
	) {
		gclass_conf->custom = xcalloc(1, gclass_new->config_size);

		if (gclass_conf != NULL)
		{
			gclass_conf->gclass = gclass_new;

			if (gclass_conf->gclass->api.config_init != NULL)
			{
				(gclass_conf->gclass->api.config_init)(gclass_conf);
			}

			success = TRUE;
		}
	}

	return (success);
}

static void xgate_class_config_deinit(xgate_class_config_t *gclass_conf)
{
	if (   (gclass_conf != NULL)
		&& (gclass_conf->gclass != NULL)
	) {
		if (gclass_conf->gclass->api.config_deinit != NULL)
		{
			(gclass_conf->gclass->api.config_deinit)(gclass_conf);
		}

		if (gclass_conf->custom != NULL)
		{
			xfree(gclass_conf->custom);
			gclass_conf->custom = NULL;
		}

		gclass_conf->gclass = NULL;
	}
}

static bool_t xgate_class_config_setup(xgate_class_config_t *gclass_conf, uint16_t argc, char * const * argv)
{
	bool_t update = FALSE;

	if (   (gclass_conf != NULL)
		&& (gclass_conf->gclass->api.config_setup != NULL)
	) {
		xcommand_arg_t arg;

		/* 共通オプション */
		for (; argc > 0; argc--, argv++)
		{
			if (xcommand_text2arg(*argv, &arg))
			{
				update |= (gclass_conf->gclass->api.config_setup)(gclass_conf, &arg);
			}
		}
	}

	return (update);
}

static bool_t xgate_class_config_clone(xgate_class_config_t *gclass_conf_dst, const xgate_class_config_t *gclass_conf_src)
{
	bool_t success = FALSE;

	if (   (gclass_conf_dst != NULL)
		&& (gclass_conf_src != NULL)
	) {
		*gclass_conf_dst = *gclass_conf_src;

		/* Deep copy */
		if (gclass_conf_dst->gclass != NULL)
		{
			gclass_conf_dst->custom = malloc(gclass_conf_dst->gclass->config_size);
			if (gclass_conf_dst->custom != NULL)
			{
				memcpy(gclass_conf_dst->custom, gclass_conf_src->custom, gclass_conf_dst->gclass->config_size);
				success = TRUE;
			}
		}
		else
		{
			success = TRUE;
		}
	}

	return (success);
}

static void xgate_class_config_print(xgate_class_config_t *gclass_conf, enum xprompt_attr attr)
{
	if (   (gclass_conf != NULL)
		&& (gclass_conf->gclass != NULL)
		&& (gclass_conf->gclass->api.config_print != NULL)
	) {
		(gclass_conf->gclass->api.config_print)(gclass_conf, attr);
	}
}

static void xgate_next_class_config_set(xgate_object_t *gobj, const xgate_class_config_t *gclass_conf_next)
{
	if (gobj != NULL)
	{
		xgate_class_config_t config_old;

		xmutex_lock(g_xgate.mtx_gate_object, XTIMEOUT_INFINITE);
		{
			config_old = gobj->class_config_next;
			gobj->class_config_next = *gclass_conf_next;
		}
		xmutex_unlock(g_xgate.mtx_gate_object);

		/* 処理されなかった古いコンフィグを削除 */
		xgate_class_config_deinit(&config_old);
	}
}

static bool_t xgate_next_class_config_get(xgate_object_t *gobj, xgate_class_config_t *gclass_conf_next)
{
	bool_t get_ok = FALSE;

	if (   (gobj != NULL)
		&& (gobj->class_config_next.gclass != NULL)
		&& (gclass_conf_next != NULL)
	) {
		xmutex_lock(g_xgate.mtx_gate_object, XTIMEOUT_INFINITE);
		{
			*gclass_conf_next = gobj->class_config_next;
			gobj->class_config_next.gclass = NULL;
		}
		xmutex_unlock(g_xgate.mtx_gate_object);

		get_ok = TRUE;
	}

	return (get_ok);
}

static void xgate_object_init(xgate_object_t *gobj, uint8_t gate_no)
{
	/* ゲート名 */
	snprintf(gobj->config.name, XCOUNTOF(gobj->config.name), "GATE_%03d", gate_no);
}

static void xgate_object_init_all(void)
{
	uint8_t index;

	/* オブジェクト名初期化 */
	for (index = 0; index < XCOUNTOF(g_xgate.obj_list); index++)
	{
		xgate_object_init(&g_xgate.obj_list[index], index);
	}
}

static enum xgate_open_status xgate_object_open_status_get(xgate_object_t *gobj)
{
	enum xgate_open_status status = XGATE_CLOSED;

	if (gobj != NULL)
	{
		if (XGATE_OBJECT_IS_VALID(gobj))
		{
			status = gobj->status.open_status;
		}
	}

	return (status);
}

static bool_t xgate_object_open(xgate_object_t *gobj)
{
	bool_t success = FALSE;

	if (   (gobj != NULL)
		&& (gobj->class_config.gclass != NULL)
	) {
		gobj->class_obj.gclass = gobj->class_config.gclass;

		/* クラスオブジェクト生成 */
		if (gobj->class_obj.custom == NULL)
		{
			gobj->class_obj.custom = xcalloc(1, gobj->class_config.gclass->object_size);
		}

		/* クラスオブジェクトのオープン処理 */
		if (gobj->class_obj.custom != NULL)
		{
			if (gobj->class_config.gclass->api.gate_open != NULL)
			{
				success = (gobj->class_config.gclass->api.gate_open)(gobj);
			}
			else
			{
				success = TRUE;
			}
		}
	}

	return (success);
}

static bool_t xgate_object_close(xgate_object_t *gobj)
{
	bool_t success = TRUE;

	if (gobj != NULL)
	{
		/* クラスオブジェクトのクローズ処理 */
		if (   (gobj->class_config.gclass != NULL)
			&& (gobj->class_config.gclass->api.gate_close != NULL)
		) {
			success = (gobj->class_config.gclass->api.gate_close)(gobj);
		}

		/* クローズ処理が成功したときにクラスオブジェクトを解放 */
		if (   (success)
			&& (gobj->class_obj.custom != NULL)
		) {
			xfree(gobj->class_obj.custom);
			gobj->class_obj.custom = NULL;
		}
	}

	return (success);
}

static void xgate_object_poll(xgate_object_t *gobj)
{
	if (   (gobj->class_config.gclass != NULL)
		&& (gobj->class_config.gclass->api.gate_poll != NULL)
	) {
		(gobj->class_config.gclass->api.gate_poll)(gobj);
	}
}

static void xgate_object_open_proc(xgate_object_t *gobj)
{
	/* Close => Opening */
	if (gobj->status.open_status < XGATE_OPENED)
	{
		switch (gobj->status.open_status)
		{
			case XGATE_OPENING_BUSY:
				if (xgate_object_open(gobj))
				{
					/* Open Success */
					gobj->status.open_status++;
				}
				else
				{
					/* オープンに失敗したときはクローズしてからトライ */
					gobj->reopen_req = TRUE;
				}
				break;
			default:
				gobj->status.open_status++;
				break;
		}
	}

	/* Opened */
	else
	{
		xgate_object_poll(gobj);
	}
}

static void xgate_object_close_proc(xgate_object_t *gobj)
{
	/* Opened => Closing */
	if (gobj->status.open_status > XGATE_CLOSED)
	{
		switch (gobj->status.open_status)
		{
			case XGATE_CLOSING_BUSY:
				if (xgate_object_close(gobj))
				{
					/* Open Success */
					gobj->status.open_status--;
				}
				break;
			default:
				gobj->status.open_status--;
				break;
		}
	}
}

static void xgate_object_config_update_proc(xgate_object_t *gobj)
{
	xgate_class_config_t config_next;

	/* 新しいコンフィグを取得 */
	if (xgate_next_class_config_get(gobj, &config_next))
	{
		/* 古いコンフィグを削除 */
		xgate_class_config_deinit(&gobj->class_config);

		/* 新しいコンフィグへ切り替え */
		gobj->class_config = config_next;
	}

	gobj->reopen_req = FALSE;
}


static xgate_class_t *xgate_class_find(const char *class_name)
{
	xgate_class_t *class_ptr = NULL;

	if (class_name != NULL)
	{
		uint8_t index;

		for (index = 0; index < XCOUNTOF(g_xgate.class_list); index++)
		{
			if (strcmp(class_name, g_xgate.class_list[index].name) == 0)
			{
				class_ptr = &g_xgate.class_list[index];
				break;
			}
		}
	}

	return (class_ptr);
}

static void xgate_forwarding(uint8_t gate_no, const uint8_t *packet_data, size_t packet_data_size)
{
	uint32_t dst_gate_mask = g_xgate.forwarding_to_gate_mask[gate_no];

	if (dst_gate_mask > 0)
	{
		uint8_t gate_no;

		for (gate_no = 0; gate_no < XCOUNTOF(g_xgate.obj_list); gate_no++)
		{
			/* 転送先リストに含まれるゲートに転送 */
			if (((dst_gate_mask >> gate_no) & 0x01) != 0)
			{
				xgate_send(gate_no, packet_data, packet_data_size);
			}
		}
	}
}

void xgate_init(void)
{
	uint8_t index;

	g_xgate.mtx_gate_object = xmutex_new();

	/* クラス情報取得 */
	for (index = 0; index < XCOUNTOF(XGATE_CLASS_LIST); index++)
	{
		/* クラスモジュールから情報取得 */
		(XGATE_CLASS_LIST[index])(&g_xgate.class_list[index]);
	}

	/* オブジェクト初期化 */
	xgate_object_init_all();
}

void xgate_deinit(void)
{
}

void xgate_poll(void)
{
	xgate_object_t *		gobj;
	enum xgate_open_status	status_prev;

	XFOREACH_ARRAY_BEGIN(gobj, g_xgate.obj_list)
	{
		do
		{
			status_prev = gobj->status.open_status;

			/* Open */
			if (   (gobj->open_req)
				&& (!gobj->reopen_req)
				&& (gobj->class_config.gclass != NULL)
				&& (gobj->class_config_next.gclass == NULL)
			) {
				xgate_object_open_proc(gobj);
			}

			/* Close */
			else
			{
				xgate_object_close_proc(gobj);
			}

			/* Update */
			if (gobj->status.open_status == XGATE_CLOSED)
			{
				xgate_object_config_update_proc(gobj);
			}

		} while (status_prev != gobj->status.open_status);
	}
	XFOREACH_ARRAY_END()
}

void xgate_class_enumeration(void (*callback)(const xgate_class_t *class_info, void *arg), void *arg)
{
	if (callback != NULL)
	{
		xgate_class_t *class_info;

		XFOREACH_ARRAY_BEGIN(class_info, g_xgate.class_list)
		{
			/* コールバックにて通知 */
			(callback)(class_info, arg);
		}
		XFOREACH_ARRAY_END()
	}
}

void xgate_object_enumeration(void (*callback)(uint8_t gate_no, const xgate_object_t *gobj, void *arg), void *arg)
{
	if (callback != NULL)
	{
		xmutex_lock(g_xgate.mtx_gate_object, XTIMEOUT_INFINITE);
		{
			xgate_object_t *gobj;
			uint8_t gate_no = 0;

			XFOREACH_ARRAY_BEGIN(gobj, g_xgate.obj_list)
			{
				/* コールバックにて通知 */
				(callback)(gate_no, gobj, arg);

				gate_no++;
			}
			XFOREACH_ARRAY_END()
		}
		xmutex_unlock(g_xgate.mtx_gate_object);
	}
}

bool_t xgate_forwarding_rule_get(uint8_t src_gate_no, uint32_t *dst_gate_mask)
{
	bool_t get_ok = FALSE;

	if (   (src_gate_no < XCOUNTOF(g_xgate.forwarding_to_gate_mask))
		&& (dst_gate_mask != NULL)
	) {
		*dst_gate_mask = g_xgate.forwarding_to_gate_mask[src_gate_no];

		get_ok = TRUE;
	}

	return (get_ok);
}

bool_t xgate_forwarding_rule_set(uint8_t src_gate_no, uint32_t dst_gate_mask)
{
	bool_t set_ok = FALSE;

	if (src_gate_no < XCOUNTOF(g_xgate.forwarding_to_gate_mask))
	{
		uint8_t gate_no;

		/* 転送先ゲートを更新 */
		g_xgate.forwarding_to_gate_mask[src_gate_no] = dst_gate_mask;

		/* 転送元ゲートを更新 */
		for (gate_no = 0; gate_no < XCOUNTOF(g_xgate.forwarding_from_gate_mask); gate_no++)
		{
			if (((dst_gate_mask >> gate_no) & 1u) != 0)
			{
				g_xgate.forwarding_from_gate_mask[gate_no] |= (uint32_t)(1u << src_gate_no);
			}
			else
			{
				g_xgate.forwarding_from_gate_mask[gate_no] &= ~(uint32_t)(1u << src_gate_no);
			}
		}

		set_ok = TRUE;
	}

	return (set_ok);
}

void xgate_monitor_reset(void)
{
	uint8_t gate_no;

	for (gate_no = 0; gate_no < XCOUNTOF(g_xgate.obj_list); gate_no++)
	{
		memset(g_xgate.obj_list[gate_no].monitor_channel, 0, sizeof(g_xgate.obj_list[gate_no].monitor_channel));
	}
}

void xgate_monitor_clear(uint8_t ch)
{
	uint8_t gate_no;
	uint8_t contents;
	xgate_object_t *gobj;

	for (gate_no = 0; gate_no < XCOUNTOF(g_xgate.obj_list); gate_no++)
	{
		gobj = &g_xgate.obj_list[gate_no];

		for (contents = 0; contents < XCOUNTOF(gobj->monitor_channel); contents++)
		{
			if (gobj->monitor_channel[contents] == (ch + 1))
			{
				gobj->monitor_channel[contents] = 0;
			}
		}
	}
}

bool_t xgate_monitor_get(uint8_t ch, uint8_t *gate_no, enum xgate_monitor_contents *contents)
{
	if ((gate_no != NULL) && (contents != NULL))
	{
		uint8_t gate_no_temp;
		uint8_t contents_temp;
		xgate_object_t *gobj;

		for (gate_no_temp = 0; gate_no_temp < XCOUNTOF(g_xgate.obj_list); gate_no_temp++)
		{
			gobj = &g_xgate.obj_list[gate_no_temp];

			for (contents_temp = 0; contents_temp < XCOUNTOF(gobj->monitor_channel); contents_temp++)
			{
				if (gobj->monitor_channel[contents_temp] > 0)
				{
					*gate_no = gate_no_temp;
					*contents = contents_temp;

					return (TRUE);
				}
			}
		}
	}

	return (FALSE);
}

bool_t xgate_monitor_set(uint8_t ch, uint8_t gate_no, enum xgate_monitor_contents contents)
{
	bool_t set_ok = FALSE;

	/* 古いモニター設定を削除 */
	xgate_monitor_clear(ch);

	if (   (gate_no < XCOUNTOF(g_xgate.obj_list))
		&& (contents < XGATE_MONITOR_CONTENTS_MAX)
	) {
		/* 新しいモニター設定を反映 */
		g_xgate.obj_list[gate_no].monitor_channel[contents] = ch + 1;

		set_ok = TRUE;
	}

	return (set_ok);
}

bool_t xgate_setup(uint8_t gate_no, const char *class_name, const char *obj_name, const char *command)
{
	xcommand_parse_buffer_t	parse_buff;
	xcommand_parse_result_t	parse_result;
	bool_t					success = FALSE;

	if (xcommand_parse(command, &parse_buff, &parse_result))
	{
		success = xgate_vsetup(gate_no, class_name, obj_name, parse_result.argc, parse_result.argv);
	}

	return (success);
}

bool_t xgate_vsetup(uint8_t gate_no, const char *class_name, const char *obj_name, uint16_t paramc, char * const * paramv)
{
	bool_t setup_ok = FALSE;

	if (gate_no < XCOUNTOF(g_xgate.obj_list))
	{
		xgate_object_t *		gobj = &g_xgate.obj_list[gate_no];
		xgate_class_t *			gclass_new = NULL;
		xgate_class_config_t	gclass_conf_new = {0};
		bool_t					update = TRUE;

		setup_ok = TRUE;

		/* クラス取得 */
		if ((class_name != NULL) && (strlen(class_name) > 0))
		{
			gclass_new = xgate_class_find(class_name);
		}
		else if (gobj->class_config.gclass != NULL)
		{
			gclass_new = gobj->class_config.gclass;
		}

		/* ゲート名更新 */
		if ((obj_name != NULL) && (strlen(obj_name) > 0))
		{
			strncpy(gobj->config.name, obj_name, XCOUNTOF(gobj->config.name));
			gobj->config.name[XCOUNTOF(gobj->config.name) - 1] = '\0';
		}

		/* ゲートに機能割り当て */
		if (gclass_new != NULL)
		{
			/* 使用中のクラスと同じ場合は使用中のコンフィグを取得 */
			if (   (gobj->class_config.gclass == gclass_new)
				&& (xgate_class_config_clone(&gclass_conf_new, &gobj->class_config))
			) {
				update = FALSE;

			} else {
				/* 新規でコンフィグ作成 */
				xgate_class_config_init(&gclass_conf_new, gclass_new);
				update = TRUE;
			}

			/* コンフィグ更新 */
			update |= xgate_class_config_setup(&gclass_conf_new, paramc, paramv);
		}

		if (update)
		{
			xgate_next_class_config_set(gobj, &gclass_conf_new);

			/* 新しいコンフィグを反映 */
			gobj->reopen_req = TRUE;

			setup_ok = TRUE;
		}
	}

	return (setup_ok);
}

void xgate_reset(uint8_t gate_no)
{
	xgate_vsetup(gate_no, NULL, NULL, 0, NULL);
}

void xgate_print(uint8_t gate_no, enum xgate_print_contents_mask mask, enum xprompt_attr attr)
{
	if (gate_no < XCOUNTOF(g_xgate.obj_list))
	{
		xgate_object_t *gobj = &g_xgate.obj_list[gate_no];

		/* Gate config */
		if ((mask & XGATE_PCMASK_CONFIG) != 0)
		{
			xgate_class_config_print(&gobj->class_config, attr);
		}
	}
}

void xgate_open_request(uint8_t gate_no, bool_t open)
{
	if (gate_no < XCOUNTOF(g_xgate.obj_list))
	{
		xgate_object_t *gobj = &g_xgate.obj_list[gate_no];

		gobj->open_req = open;
	}
}

enum xgate_open_status xgate_open_status_get(uint8_t gate_no)
{
	enum xgate_open_status status = XGATE_CLOSED;

	if (gate_no < XCOUNTOF(g_xgate.obj_list))
	{
		status = xgate_object_open_status_get(&g_xgate.obj_list[gate_no]);
	}

	return (status);
}

void xgate_send(uint8_t gate_no, const uint8_t *data, size_t data_size)
{
	if (   (gate_no < XCOUNTOF(g_xgate.obj_list))
		&& (data != NULL)
		&& (data_size > 0)
	) {
		xgate_object_send(&g_xgate.obj_list[gate_no], data, data_size);
	}
}

void xgate_object_reopen(xgate_object_t *gobj)
{
	if (gobj != NULL)
	{
		gobj->reopen_req = TRUE;
	}
}

void xgate_object_send(xgate_object_t *gobj, const uint8_t *data, size_t data_size)
{
	if (   (gobj != NULL)
		&& (gobj->status.open_status == XGATE_OPENED)
	) {
		/* 送信処理(オブジェクト別) */
		if (   (gobj->class_config.gclass != NULL)
			&& (gobj->class_config.gclass->api.gate_send != NULL))
		{
			(gobj->class_config.gclass->api.gate_send)(gobj, data, data_size);
		}

		/* 送信カウンタ更新 */
		if (gobj->status.send_packet_counter < XGATE_PACKET_COUNTER_MAX)
		{
			gobj->status.send_packet_counter++;
		}

		/* 送信モニター */
		if (gobj->monitor_channel[XGATE_MONITOR_SEND_DATA] > 0)
		{
			xremcon_gate_monitor_write(gobj->monitor_channel[XGATE_MONITOR_SEND_DATA] - 1, data, data_size);
		}
	}
}

void xgate_object_recv(xgate_object_t *gobj, const uint8_t *data, size_t data_size)
{
	if (   (gobj != NULL)
		&& (gobj->status.open_status == XGATE_OPENED)
	) {
		uint8_t gate_no = XGATE_OBJECT_NO(gobj);

		/* 受信カウンタ更新 */
		if (gobj->status.recv_packet_counter < XGATE_PACKET_COUNTER_MAX)
		{
			gobj->status.recv_packet_counter++;
		}

		/* 受信モニター */
		if (gobj->monitor_channel[XGATE_MONITOR_RECV_DATA] > 0)
		{
			xremcon_gate_monitor_write(gobj->monitor_channel[XGATE_MONITOR_RECV_DATA] - 1, data, data_size);
		}

		/* パケット転送 */
		xgate_forwarding(gate_no, data, data_size);
	}
}

enum xgate_status_mask xgate_object_status_get(xgate_object_t *gobj)
{
	enum xgate_status_mask status_mask = 0;

	if (gobj != NULL)
	{
		status_mask = gobj->status.status_mask;
	}

	return (status_mask);
}

void xgate_object_status_reset(xgate_object_t *gobj, enum xgate_status_mask init_status)
{
	if (gobj != NULL)
	{
		enum xgate_status_mask mask_diff = gobj->status.status_mask ^ init_status;

		/* ステータス更新 */
		gobj->status.status_mask = init_status;
	}
}

void xgate_object_status_set(xgate_object_t *gobj, enum xgate_status_mask set_mask)
{
	if (gobj != NULL)
	{
		xgate_object_status_reset(gobj, gobj->status.status_mask | set_mask);
	}
}

void xgate_object_status_clear(xgate_object_t *gobj, enum xgate_status_mask status_mask)
{
	if (gobj != NULL)
	{
		xgate_object_status_reset(gobj, gobj->status.status_mask);
	}
}

