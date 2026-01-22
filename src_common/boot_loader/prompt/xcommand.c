/*
 * xcommand.c
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */


#include "xcommand.h"
#include "xcommand_bl.h"

#include "xprompt.h"

#include "core/xmalloc.h"


#include <string.h>
#include <stdlib.h>


static void xcommand_print_subcmd_help(xcommand_callstack_t *cs, const xcommand_info_t *sub_cmd_list, uint16_t sub_cmd_count, size_t indent)
{
	if (   (cs != NULL)
		&& (sub_cmd_list != NULL)
		&& (sub_cmd_count > 0)
	) {
		char	buff[XCOMMAND_LENGTH_MAX + 1];
		size_t	buff_offset;

		if (indent > (XCOUNTOF(buff) - 1)) {
			indent = XCOUNTOF(buff) - 1;
		}

		while (sub_cmd_count > 0) {
			/* インデント設定 */
			memset(buff, ' ', indent);
			buff_offset += indent;

			/* サブコマンド名 */
			strncpy(buff + indent, sub_cmd_list->name, XCOUNTOF(buff) - indent);
			xprompt_printf(cs->prompt_attr, buff);

			/* サブコマンドヘルプ */
			xprompt_print_multiline(cs->prompt_attr, indent + 2, sub_cmd_list->help, 1);

			sub_cmd_list++;
			sub_cmd_count--;
		}
	}
}

static bool_t xcommand_sub_action(xcommand_callstack_t *cs, const xcommand_info_t *sub_cmd)
{
	bool_t action = FALSE;

	if (   (cs != NULL)
		&& (sub_cmd != NULL)
	) {
		xcommand_callstack_t cs_sub = *cs;

		cs_sub.cmd = sub_cmd;

		/* サブコマンドの詳細ヘルプを表示 */
		if (   (cs_sub.argc > 0)
			&& (cs_sub.argv != NULL)
			&& (strcmp(*(cs_sub.argv), "help") == 0)
		) {
			xprompt_print_multiline(cs_sub.prompt_attr, 0, cs_sub.cmd->help, 0xFFFF);

			action = TRUE;
		}

		/* サブコマンド実行 */
		else if (cs_sub.cmd->func != NULL)
		{
			action = (cs_sub.cmd->func)(&cs_sub);
		}
	}

	return (action);
}

/* --------------------------------------------------------
 * bdinfo
 * ----------------------------------------------------- */

static bool_t xcommand_bdinfo(xcommand_callstack_t *cs)
{
	bool_t			success = TRUE;

	return (success);
}

/* =======================================================================
 * 		Command Map (Root)
 * ==================================================================== */

const xcommand_info_t XCMDLIST_ROOT[] =
{
		{ "bdinfo", xcommand_bdinfo, "Board information print."				},
		{ "bl",     xcommand_bl,     "Boot loader command."					},
};


bool_t xcommand_exec(enum xprompt_attr attr, const char *command)
{
	xcommand_parse_buffer_t	parse_buff;
	xcommand_parse_result_t	parse_result;
	xcommand_callstack_t	cs = {0};
	bool_t					success = FALSE;

	if (xcommand_parse(command, &parse_buff, &parse_result))
	{
		cs.prompt_attr = attr;
		cs.argc = parse_result.argc;
		cs.argv = parse_result.argv;

		success = xcommand_sub_call(&cs, XCMDLIST_ROOT, XCOUNTOF(XCMDLIST_ROOT));
	}

	return (success);
}

bool_t xcommand_parse(const char *command, xcommand_parse_buffer_t *buffer, xcommand_parse_result_t *result)
{
	bool_t success = FALSE;

	if (   (command != NULL)
		&& (buffer != NULL)
		&& (result != NULL)
	) {
		uint16_t work_buff_offset = 0;
		char delim = '\0';

		result->argc = 0;

		while (   (*command != '\0')
			   && (work_buff_offset < XCOUNTOF(buffer->work_buff))
			   && (result->argc < XCOUNTOF(result->argv))
		) {
			if (delim != '\0')
			{
				/* === 引数収集中 === */
				if (*command == delim)
				{
					/* 引数終端 */
					buffer->work_buff[work_buff_offset++] = '\0';
					delim = '\0';
				}
				else
				{
					buffer->work_buff[work_buff_offset++] = *command;
				}

			}
			else if (*command != ' ')
			{
				/* === 区切り検出 === */
				result->argv[result->argc++] = buffer->work_buff + work_buff_offset;

				switch (*command)
				{
					case '"':
					case '\'':
						/* 特殊区切り文字 */
						delim = *command;
						break;
					default:
						/* 通常区切り文字 */
						delim = ' ';

						/* 区切りの一文字目をバッファに追加 */
						buffer->work_buff[work_buff_offset++] = *command;
						break;
				}
			}

			command++;
		}

		/* 収集中の引数を閉じる */
		if ((delim == ' ') && (work_buff_offset < XCOUNTOF(buffer->work_buff)))
		{
			buffer->work_buff[work_buff_offset] = '\0';
		}

		success = TRUE;
	}

	return (success);

}

bool_t xcommand_sub_call(xcommand_callstack_t *cs, const xcommand_info_t *sub_cmd_list, uint16_t sub_cmd_count)
{
	bool_t action = FALSE;

	if (   (cs != NULL)
		&& (cs->argc > 0)
		&& (cs->argv != NULL)
		&& (sub_cmd_list != NULL)
		&& (sub_cmd_count > 0)
	) {
		const char *argv_text;

		if (xcommand_arg_pop_text(cs, &argv_text))
		{

			/* 現在のコマンドヘルプとサブコマンド一覧を表示 */
			if (strcmp(argv_text, "help") == 0)
			{
				/* 現在のコマンドヘルプ */
				if (cs->cmd != NULL)
				{
					xprompt_print_multiline(cs->prompt_attr, 0, cs->cmd->help, 0xFFFF);
				}

				/* サブコマンドヘルプ */
				xcommand_print_subcmd_help(cs, sub_cmd_list, sub_cmd_count, 0);

				action = TRUE;
			}

			/* サブコマンド処理 */
			else
			{
				uint16_t index = 0;

				for (index = 0; index < sub_cmd_count; index++)
				{
					if (strcmp(argv_text, sub_cmd_list[index].name) == 0)
					{
						action = xcommand_sub_action(cs, &sub_cmd_list[index]);
						break;
					}
				}
			}
		}
	}

	return (action);
}

bool_t xcommand_arg_pop_text(xcommand_callstack_t *cs, const char **arg)
{
	bool_t pop_ok = FALSE;

	if (   (cs != NULL)
		&& (cs->argc > 0)
		&& (arg != NULL)
	)
	{
		*arg = *(cs->argv);
		cs->argv++;
		cs->argc--;

		pop_ok = TRUE;
	}

	return (pop_ok);
}

bool_t xcommand_arg_pop(xcommand_callstack_t *cs, xcommand_arg_t *arg)
{
	bool_t pop_ok = FALSE;

	if (arg != NULL)
	{
		const char *argv_text;

		if (xcommand_arg_pop_text(cs, &argv_text))
		{
			if (xcommand_text2arg(argv_text, arg))
			{
				pop_ok = TRUE;
			}
		}
	}

	return (pop_ok);
}

bool_t xcommand_text2arg(const char *text, xcommand_arg_t *arg)
{
	bool_t success = FALSE;

	if (   (text != NULL)
		&& (arg != NULL)
	)
	{
		size_t collect_len = 0;

		/* Collect Name */
		while ((*text != '\0') && (*text != '=') && (collect_len < XCOUNTOF(arg->name)))
		{
			arg->name[collect_len++] = *text++;
		}

		/* =の場合は1文字進める */
		if (*text != '\0')
		{
			text++;
		}

		/* nameを確実に収集できたときのみ成功 */
		if (collect_len < XCOUNTOF(arg->name))
		{
			/* nameを終端 */
			arg->name[collect_len++] = '\0';

			/* Collect Value */
			collect_len = 0;
			while ((*text != '\0') && (collect_len < XCOUNTOF(arg->value)))
			{
				arg->value[collect_len++] = *text++;
			}

			/* valueを終端 */
			arg->value[collect_len++] = '\0';

			success = TRUE;
		}
	}

	return (success);
}

bool_t xcommand_text2bin(const char *text, uint8_t *buffer, size_t buffer_size, size_t *result_size)
{
	enum bintext_parse_mode
	{
		BINTEXT_PARSE_MODE_HEX,
		BINTEXT_PARSE_MODE_BIN,
		BINTEXT_PARSE_MODE_TEXT,
	};

	bool_t		success = TRUE;
	size_t		conv_size = 0;

	enum bintext_parse_mode		mode = 0;
	enum bintext_parse_mode		mode_next = mode;

	uint8_t						value = 0;
	uint8_t						value_offset = 0;
	uint8_t						value_temp;
	uint8_t						value_bitlen;
	char						text_mode_code = '\0';

	while (*text != '\0')
	{
		/* モード変更 */
		if (mode != mode_next)
		{
			mode = mode_next;

			if (value_offset > 0)
			{
				/* 既に数値解析中の場合は解析中の値を格納 */
				if (conv_size < buffer_size)
				{
					*buffer++ = value;
				}
				conv_size++;

				/* 解析中データクリア */
				value = 0;
				value_offset = 0;
			}
		}

		if (mode == BINTEXT_PARSE_MODE_TEXT)
		{
			/* テキストモード */
			if (*text != text_mode_code)
			{
				/* 文字をそのまま格納 */
				if (conv_size < buffer_size)
				{
					*buffer++ = *text;
				}
				conv_size++;

			}
			else
			{
				/* テキストモード終了 */
				mode_next = 0;
			}

		}
		else if (*text == ' ')
		{
			/* 区切り文字 */
			if (value_offset > 0)
			{
				/* 既に数値解析中の場合は解析中の値を格納 */
				if (conv_size < buffer_size)
				{
					*buffer++ = value;
				}
				conv_size++;

				/* 解析中データクリア */
				value = 0;
				value_offset = 0;
			}

		}
		else if ((*text == '\'') || (*text == '\"'))
		{
			/* テキストモードへ移行 */
			mode_next = BINTEXT_PARSE_MODE_TEXT;

			/* テキストモードへ移行した文字を記憶 */
			text_mode_code = *text;

		}
		else if (*text == '$')
		{
			/* 16進数モードへ移行 */
			mode_next = BINTEXT_PARSE_MODE_HEX;
		}
		else if (*text == '#')
		{
			/* 2進数モードへ移行 */
			mode_next = BINTEXT_PARSE_MODE_BIN;

		}
		else
		{
			if (mode == BINTEXT_PARSE_MODE_HEX)
			{
				/* 16進数モード */
				value_bitlen = 4;
				if ((*text >= '0') && (*text <= '9'))
				{
					value_temp = (uint8_t)(*text - '0');
				}
				else if ((*text >= 'a') && (*text <= 'f'))
				{
					value_temp = (uint8_t)(*text - 'a' + 10);
				}
				else if ((*text >= 'A') && (*text <= 'F'))
				{
					value_temp = (uint8_t)(*text - 'A' + 10);
				}
				else
				{
					/* 16進数ではない */
					success = FALSE;
					break;
				}

			}
			else if (mode == BINTEXT_PARSE_MODE_BIN)
			{
				/* 2進数モード */
				value_bitlen = 1;
				if (*text == '0')
				{
					value_temp = 0;
				}
				else if (*text == '1')
				{
					value_temp = 1;
				}
				else
				{
					/* 2進数ではない */
					success = FALSE;
					break;
				}
			}
			else
			{
				/* 不明なモード */
				success = FALSE;
				break;
			}

			value <<= value_bitlen;
			value |= value_temp;
			value_offset += value_bitlen;

			if (value_offset >= 8)
			{
				/* 8bit以上集まった場合は格納 */
				if (conv_size < buffer_size)
				{
					*buffer++ = value;
				}
				conv_size++;

				/* 解析中データクリア */
				value = 0;
				value_offset = 0;
			}
		}

		text++;
	}

	if (success)
	{
		if (value_offset > 0)
		{
			/* 既に収集中の場合は現データを格納 */
			if (conv_size < buffer_size)
			{
				*buffer++ = value;
			}
			conv_size++;
		}

		if (result_size != NULL)
		{
			*result_size = conv_size;
		}
	}

	return (success);
}

uint32_t xcommand_text2uint(const char *text)
{
	uint32_t value = 0;

	if (text != NULL)
	{
		int base = 0;
		char *endptr;

		if (*text == 'b')
		{
			base = 2;
			text++;
		}

		value = strtol(text, &endptr, base);
	}

	return (value);
}

