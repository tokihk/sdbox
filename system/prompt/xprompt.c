/*
 * xprompt.c
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */


#include <config/xconfig_const.h>
#include "xprompt.h"
#include "xcommand.h"

#include "core/xtask.h"

#include "driver/xremcon.h"

#include "thirdparty/ntshell/ntshell.h"

#include <stdio.h>
#include <stdarg.h>


#define XPROMPT_BASIC_TEXT				XCONF_PROGRAM_NAME"> "

#define XPROMPT_BINARY_OUT_BLOCK_SIZE	(16)

#define XPROMPT_NEW_LINE				NTSHELL_PROMPT_NEWLINE


static struct
{
	xtask_t						task_main;

	xmutex_t					mtx_record;

	xprompt_record_t			record_list[XPROMPT_RECORD_NUM];
	uint16_t					record_ptr_in;

	ntshell_t					ntshell_obj;

	bool_t						init_state;
} g_xprompt;


static void xprompt_data_write(const uint8_t *data, size_t data_size, bool_t sync_mode)
{
	size_t write_size;

	do
	{
		write_size = xremcon_terminal_write((const uint8_t *)data, data_size);

		data_size -= write_size;
		data += write_size;
	}
	while ((sync_mode) && (write_size < data_size));
}

static const char *xprompt_text_line_get(const char *text, char *line_buffer, size_t line_buffer_size)
{
	if (   (text != NULL)
		&& (line_buffer != NULL)
		&& (line_buffer_size > 0)
	) {
		while ((*text != '\0') && (*text != '\n'))
		{
			/* バッファに空きがあるときのみコピー */
			if (line_buffer_size > 1)
			{
				*line_buffer = *text;
				line_buffer++;
				line_buffer_size--;
			}

			text++;
		}

		/* 終端 */
		*line_buffer = '\0';

		if (*text == '\n')
		{
			text++;
		}
	}

	return (text);
}

static struct xprompt_record *xprompt_command_record_block_get(void)
{
	xprompt_record_t *record = NULL;

	record = &g_xprompt.record_list[g_xprompt.record_ptr_in];

	if (g_xprompt.record_ptr_in < (XCOUNTOF(g_xprompt.record_list) - 1))
	{
		g_xprompt.record_ptr_in++;
	}
	else
	{
		g_xprompt.record_ptr_in = 0;
	}

	return (record);
}

static void xprompt_command_record(const char *text)
{
	if (text != NULL)
	{
		xprompt_record_t *record;

		record = xprompt_command_record_block_get();
		if (record != NULL)
		{
			strncpy(record->text, text, XCOUNTOF(record->text));
		}
	}
}

static void xprompt_vprintf(enum xprompt_attr attr, const char *format, va_list args)
{
	if (   (format != NULL)
		&& ((attr & XPROMPT_ATTR_SILENT) == 0)
	) {
		char	text[XPROMPT_LINE_TEXT_LEN + 1];
		size_t	text_len;

		text_len = (size_t)vsnprintf(text, XCOUNTOF(text), format, args);

		/* 実行コマンドを記録 */
		if ((attr & XPROMPT_ATTR_RECORD) != 0)
		{
			xprompt_command_record(text);
		}

		xprompt_data_write((const uint8_t *)text, text_len, TRUE);
		xprompt_data_write((const uint8_t *)XPROMPT_NEW_LINE, XCOUNTOF(XPROMPT_NEW_LINE) - 1, TRUE);
	}
}

static int ntshell_user_callback(const char *text, void *extobj)
{
	int ret = 0;

	/* text長が0ではないときのみ処理 */
	if (*text != '\0')
	{
		/* 入力された実行コマンドを記録 */
		{
			char record_text[XPROMPT_LINE_TEXT_LEN + 1];

			snprintf(record_text, XCOUNTOF(record_text), "> %s", text);

			xprompt_command_record(record_text);
		}

		/* コマンド実行 */
		ret = xcommand_exec(XPROMPT_ATTR_RECORD, text);

		/* エラー出力 */
		if (!ret)
		{
			xprompt_printf(XPROMPT_ATTR_RECORD, "Command error.");
		}
	}

	return (ret);
}

static int ntshell_data_read(char *buf, int cnt, void *extobj)
{
	size_t read_size = 0;

	/* UARTからデータ取得 */
	read_size = xremcon_terminal_read((uint8_t *)buf, (uint16_t)cnt);

	/* 標準ライブラリと同じく終端を-1で閉じる */
	if (read_size < (size_t)cnt)
	{
		buf[read_size] = -1;
	}

	return ((int)read_size);
}

static int ntshell_data_write(const char *buf, int cnt, void *extobj)
{
	return ((int)xremcon_terminal_write((const uint8_t *)buf, (size_t)cnt));
}

void xprompt_init(void)
{
	g_xprompt.mtx_record = xmutex_new();

	g_xprompt.init_state = TRUE;

	/* 起動ログ */
	xprompt_printf(0, "");
	xprompt_printf
	(
			XPROMPT_ATTR_RECORD,
			"<--- %s %d.%d.%d.%s start --->",
			XCONF_PROGRAM_NAME,
			XCONF_PROGRAM_VERSION_MAJOR,
			XCONF_PROGRAM_VERSION_MINOR,
			XCONF_PROGRAM_VERSION_BUGFIX,
			XCONF_PROGRAM_VERSION_CUSTOM
	);

	/* ntshell初期化 */
	ntshell_init(&g_xprompt.ntshell_obj, ntshell_data_read, ntshell_data_write, ntshell_user_callback, NULL);
	ntshell_set_prompt(&g_xprompt.ntshell_obj, XPROMPT_BASIC_TEXT);
	ntshell_startup(&g_xprompt.ntshell_obj);
}

void xprompt_poll(void)
{
	ntshell_poll(&g_xprompt.ntshell_obj);
}

/* 最も新しいレコードから順番に列挙する */
void xprompt_record_enumeration(bool_t (*callback)(uint16_t index, const xprompt_record_t *record, void *arg), void *arg)
{
	if (callback != NULL)
	{
		uint16_t index;
		uint16_t record_ptr = g_xprompt.record_ptr_in;

		for (index = 0; index < XCOUNTOF(g_xprompt.record_list); index++)
		{
			/* レコードリスト参照位置取得 */
			if (record_ptr > 0)
			{
				record_ptr--;
			}
			else
			{
				record_ptr = XCOUNTOF(g_xprompt.record_list) - 1;
			}

			/* 列挙結果参照用コールバック */
			if (!(*callback)(index, &g_xprompt.record_list[record_ptr], arg))
			{
				/* FALSEが返ってきたときは即座に終了 */
				break;
			}
		}
	}
}

void xprompt_printf(enum xprompt_attr attr, const char *format, ... )
{
	if (   (format != NULL)
		&& (g_xprompt.init_state)
	) {
		va_list args = {0};

		/* 可変引数初期化 */
		va_start(args, format);

		/* 処理開始 */
		xprompt_vprintf(attr, format, args);

		/* 可変引数終了 */
		va_end(args);
	}
}

void xprompt_print_bin(enum xprompt_attr attr, const uint8_t *data, size_t data_size)
{
	if (   (data != NULL)
		&& (data_size > 0)
		&& (g_xprompt.init_state)
	) {
		uint8_t line_data_size;
		uint8_t line_data_pos;
		char    line_text_data[XPROMPT_BINARY_OUT_BLOCK_SIZE * 3 + 1];
		char *  line_text_ptr;

		while (data_size > 0)
		{
			line_data_size = XMIN(data_size, XPROMPT_BINARY_OUT_BLOCK_SIZE);

			/* 1行文字列生成 */
			line_text_ptr = line_text_data;
			for (line_data_pos = 0; line_data_pos < line_data_size; line_data_pos++)
			{
				(*line_text_ptr++) = *("0123456789ABCDEF" + (size_t)((*data >> 4) & 0x0F));
				(*line_text_ptr++) = *("0123456789ABCDEF" + (size_t)((*data >> 0) & 0x0F));
				(*line_text_ptr++) = ' ';

				data++;
			}
			(*line_text_ptr) = '\0';

			xprompt_printf(attr, "%s", line_text_data);

			data_size -= line_data_size;
		}
	}
}

void xprompt_print_multiline(enum xprompt_attr attr, size_t indent, const char *text, size_t text_line_num)
{
	if (text != NULL)
	{
		char buff[XPROMPT_LINE_TEXT_LEN + 1];

		if (indent > (XCOUNTOF(buff) - 1))
		{
			indent = XCOUNTOF(buff) - 1;
		}

		while ((*text != '\0') && (text_line_num > 0))
		{
			/* インデント設定 */
			memset(buff, ' ', indent);

			/* 1行読み込み */
			text = xprompt_text_line_get(text, buff + indent, XCOUNTOF(buff) - indent);

			/* 出力 */
			xprompt_printf(attr, buff);

			text_line_num--;
		}
	}
}

void xprompt_debug_printf(const char *format, ... )
{
	if ((format != NULL)  && (g_xprompt.init_state))
	{
		va_list args = {0};

		/* 可変引数初期化 */
		va_start(args, format);

		/* 処理開始 */
		xprompt_vprintf(0, format, args);

		/* 可変引数終了 */
		va_end(args);
	}
}

void xprompt_debug_print_bin(const uint8_t *data, size_t data_size)
{
	xprompt_print_bin(0, data, data_size);
}

