/*
 * xcommand_gate.c
 *
 *  Created on: 2024/08/27
 *      Author: Hitoshi Kono
 */

#include "xcommand_gate.h"
#include "xcommand.h"
#include "xprompt.h"

#include "system/gate/xgate.h"


const char XCOMMAND_GATE_EMPTY_TEXT[] = "(Empty)";

const char *XCOMMAND_GATE_OPEN_STATUS_TEXT[] = {
		"Close",
		"Close Busy",
		"Open Busy",
		"Open"
};


/* --------------------------------------------------------
 * gate setup [no=] [class=] [name=] [property1] [property2] ...
 * ----------------------------------------------------- */

static bool_t xcommand_gate_setup(xcommand_callstack_t *cs)
{
	bool_t					success = TRUE;
	xcommand_callstack_t	cs_backup = *cs;
	const char *			argv_text;
	xcommand_arg_t			arg;

	uint8_t					gate_no;
	char					class_name[XGATE_CLASS_NAME_LEN_MAX + 1] = {0};
	char					gate_name[XGATE_OBJECT_NAME_LEN_MAX + 1] = {0};

	/* 0: gate_no */
	if ((success) && (success = xcommand_arg_pop_text(cs, &argv_text)))
	{
		gate_no = (uint8_t)atoi(argv_text);
	}

	/* クラス名/ゲート名の指定を検索 */
	while (xcommand_arg_pop(cs, &arg))
	{
		/* class */
		if (strcmp(arg.name, "class") == 0)
		{
			strncpy(class_name, arg.value, XCOUNTOF(class_name));
			class_name[XCOUNTOF(class_name) - 1] = '\0';
		}

		/* name */
		else if (strcmp(arg.name, "name") == 0)
		{
			strncpy(gate_name, arg.value, XCOUNTOF(gate_name));
			gate_name[XCOUNTOF(gate_name) - 1] = '\0';
		}
	}

	success = xgate_vsetup(gate_no, class_name, gate_name, cs_backup.argc, cs_backup.argv);

	return (success);
}

/* --------------------------------------------------------
 * gate release [gate_no]
 * ----------------------------------------------------- */

static bool_t xcommand_gate_release(xcommand_callstack_t *cs)
{
	bool_t			success = TRUE;
	const char *	argv_text;

	uint8_t			gate_no;

	/* 0: gate_no */
	if ((success) && (success = xcommand_arg_pop_text(cs, &argv_text)))
	{
		gate_no = (uint8_t)atoi(argv_text);
	}

	if (success)
	{
		xgate_reset(gate_no);
	}

	return (success);
}

/* --------------------------------------------------------
 * gate open [gate_no]
 * ----------------------------------------------------- */

static bool_t xcommand_gate_open(xcommand_callstack_t *cs)
{
	bool_t			success = TRUE;
	const char *	argv_text;

	uint8_t			gate_no;

	/* 0: gate_no */
	if ((success) && (success = xcommand_arg_pop_text(cs, &argv_text)))
	{
		gate_no = (uint8_t)atoi(argv_text);
	}

	if (success)
	{
		xgate_open_request(gate_no, TRUE);
	}

	return (success);
}

/* --------------------------------------------------------
 * gate close [gate_no]
 * ----------------------------------------------------- */

static bool_t xcommand_gate_close(xcommand_callstack_t *cs)
{
	bool_t			success = TRUE;
	const char *	argv_text;

	uint8_t			gate_no;

	/* 0: gate_no */
	if ((success) && (success = xcommand_arg_pop_text(cs, &argv_text)))
	{
		gate_no = (uint8_t)atoi(argv_text);
	}

	if (success)
	{
		xgate_open_request(gate_no, FALSE);
	}

	return (success);
}

/* --------------------------------------------------------
 * gate forwarding [src_gate_no] [dst_gate_mask]
 * ----------------------------------------------------- */

static bool_t xcommand_gate_forwarding(xcommand_callstack_t *cs)
{
	bool_t						success = TRUE;
	const char *				argv_text;

	uint8_t						src_gate_no = 0;
	uint32_t					dst_gate_mask = 0;

	/* 0: src_gate_no */
	if ((success) && (success = xcommand_arg_pop_text(cs, &argv_text)))
	{
		src_gate_no = (uint8_t)atoi(argv_text);
	}

	/* 1: dst_gate_mask */
	if ((success) && (success = xcommand_arg_pop_text(cs, &argv_text)))
	{
		dst_gate_mask = xcommand_text2uint(argv_text);
	}

	if (success)
	{
		success = xgate_forwarding_rule_set(src_gate_no, dst_gate_mask);
	}

	return (success);
}

/* --------------------------------------------------------
 * gate list
 * ----------------------------------------------------- */

static void xcommand_gate_list_enum(uint8_t gate_no, const xgate_object_t *gobj, void *arg)
{
	xcommand_callstack_t *cs = (xcommand_callstack_t *)arg;

	const char *	class_name = XCOMMAND_GATE_EMPTY_TEXT;
	const char *	open_status_text = XCOMMAND_GATE_OPEN_STATUS_TEXT[0];

	if (XGATE_OBJECT_IS_VALID(gobj))
	{
		class_name = gobj->class_config.gclass->name;
		open_status_text  = XCOMMAND_GATE_OPEN_STATUS_TEXT[gobj->status.open_status];
	}

	/* GateNo + GateName + Open/Close + MirroringConfig */
	xprompt_printf(
		cs->prompt_attr,
		"%d: [%-12s] %s - %s",
		gate_no,
		gobj->config.name,
		class_name,
		open_status_text
	);

	if (XGATE_OBJECT_IS_VALID(gobj))
	{
		/* Counter */
		xprompt_printf(
			cs->prompt_attr,
			"  Counter: recv=%d send=%d",
			gobj->status.recv_packet_counter,
			gobj->status.send_packet_counter
		);

		xgate_print(gate_no, XGATE_PCMASK_ALL, cs->prompt_attr);
	}

	xprompt_printf(cs->prompt_attr, "");
}

static bool_t xcommand_gate_list(xcommand_callstack_t *cs)
{
	xgate_object_enumeration(xcommand_gate_list_enum, cs);

	return (TRUE);
}

/* --------------------------------------------------------
 * gate class list
 *   全クラスのヘルプを表示
 * gate class [class_name]
 *   指定したクラスの全ヘルプを表示
 * ----------------------------------------------------- */

typedef struct xcommand_gate_class_enum_param
{
	xcommand_callstack_t *		cs;
	const char *				target;
} xcommand_gate_class_enum_param_t;

static void xcommand_gate_class_enum(const xgate_class_t *class_info, void *arg)
{
	xcommand_gate_class_enum_param_t *param = (xcommand_gate_class_enum_param_t *)arg;

	if (   (param->target == NULL)
		|| (strcmp(param->target, class_info->name) == 0)
	) {
		/* クラス名 */
		xprompt_printf(param->cs->prompt_attr, "%s:", class_info->name);

		/* ヘルプ */
		if (class_info->help != NULL)
		{
			xprompt_print_multiline(param->cs->prompt_attr, 2, class_info->help, 0xFFFF);
		}
	}
}

static bool_t xcommand_gate_class(xcommand_callstack_t *cs)
{
	bool_t			success = FALSE;
	const char *	argv_text;

	if (xcommand_arg_pop_text(cs, &argv_text))
	{
		xcommand_gate_class_enum_param_t param = {0};

		param.cs = cs;
		param.target = argv_text;

		/* listのときは全クラスの情報を表示 */
		if (strcmp(argv_text, "list") == 0)
		{
			param.target = NULL;
		}

		xgate_class_enumeration(xcommand_gate_class_enum, &param);

		success = TRUE;
	}

	return (success);
}

/* --------------------------------------------------------
 * gate monitor reset
 *   ゲートの全モニターを解除する
 * ----------------------------------------------------- */
static bool_t xcommand_gate_monitor_reset(xcommand_callstack_t *cs)
{
	xgate_monitor_reset();

	return (TRUE);
}

/* --------------------------------------------------------
 * gate monitor clear [ch]
 *   ゲートモニターをクリアする
 * ----------------------------------------------------- */
static bool_t xcommand_gate_monitor_clear(xcommand_callstack_t *cs)
{
	bool_t						success = TRUE;
	const char *				argv_text;

	uint8_t						ch = 0;

	/* 0: channel */
	if ((success) && (success = xcommand_arg_pop_text(cs, &argv_text)))
	{
		ch = (uint8_t)xcommand_text2uint(argv_text);
	}

	if (success)
	{
		xgate_monitor_clear(ch);
	}

	return (success);
}

/* --------------------------------------------------------
 * gate monitor set [ch] [gate_no] send|recv
 *   ゲートの入出力データをモニターする
 * ----------------------------------------------------- */
static bool_t xcommand_gate_monitor_set(xcommand_callstack_t *cs)
{
	bool_t						success = TRUE;
	const char *				argv_text;
	xcommand_arg_t				arg;

	uint8_t						ch = 0;
	uint8_t						gate_no = 0;
	enum xgate_monitor_contents	contents = 0;

	/* 0: channel */
	if ((success) && (success = xcommand_arg_pop_text(cs, &argv_text)))
	{
		ch = (uint8_t)xcommand_text2uint(argv_text);
	}

	/* 1: gate_no */
	if ((success) && (success = xcommand_arg_pop_text(cs, &argv_text)))
	{
		gate_no = xcommand_text2uint(argv_text);
	}

	/* モニターコンテンツ */
	while (xcommand_arg_pop(cs, &arg))
	{
		/* send */
		if (strcmp(arg.name, "send") == 0)
		{
			contents = XGATE_MONITOR_SEND_DATA;
		}

		/* name */
		else if (strcmp(arg.name, "recv") == 0)
		{
			contents = XGATE_MONITOR_RECV_DATA;
		}
	}

	if (success)
	{
		success = xgate_monitor_set(ch, gate_no, contents);
	}

	return (success);
}

/* =======================================================================
 * 		Command Map (gate monitor)
 * ==================================================================== */

const xcommand_info_t XCMDLIST_GATE_MONITOR[] =
{
		{ "reset",  xcommand_gate_monitor_reset,  "" },
		{ "clear",  xcommand_gate_monitor_clear,  "" },
		{ "set",    xcommand_gate_monitor_set,    "" },
};

bool_t xcommand_gate_monitor(xcommand_callstack_t *cs)
{
	return (xcommand_sub_call(cs, XCMDLIST_GATE_MONITOR, XCOUNTOF(XCMDLIST_GATE_MONITOR)));
}

/* =======================================================================
 * 		Command Map (gate)
 * ==================================================================== */

const xcommand_info_t XCMDLIST_GATE[] =
{
		{ "setup",      xcommand_gate_setup,      "" },
		{ "release",    xcommand_gate_release,    "" },
		{ "open",       xcommand_gate_open,       "" },
		{ "close",      xcommand_gate_close,      "" },
		{ "forwarding", xcommand_gate_forwarding, "" },
		{ "list",       xcommand_gate_list,       "" },
		{ "class",      xcommand_gate_class,      "" },
		{ "monitor",    xcommand_gate_monitor,    "" },
};

bool_t xcommand_gate(xcommand_callstack_t *cs)
{
	return (xcommand_sub_call(cs, XCMDLIST_GATE, XCOUNTOF(XCMDLIST_GATE)));
}


