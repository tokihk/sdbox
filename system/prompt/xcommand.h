/*
 * xcommand.h
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */

#ifndef XCOMMAND_H_
#define XCOMMAND_H_

#include "prompt/xprompt.h"


#define XCOMMAND_LENGTH_MAX				(XPROMPT_LINE_TEXT_LEN)
#define XCOMMAND_ARG_NUM_MAX			(10)

#define XCOMMAND_ARG_NAME_LEN_MAX		(16)
#define XCOMMAND_ARG_VALUE_LEN_MAX		(64)


typedef struct xcommand_callstack xcommand_callstack_t;


typedef bool_t					(*xcommand_func_t)(xcommand_callstack_t *cs);

typedef struct xcommand_arg
{
	char						name[XCOMMAND_ARG_NAME_LEN_MAX + 1];
	char						value[XCOMMAND_ARG_VALUE_LEN_MAX + 1];
} xcommand_arg_t;

typedef struct xcommand_info
{
	const char *				name;
	xcommand_func_t				func;
	const char *				help;
} xcommand_info_t;

typedef struct xcommand_callstack
{
	enum xprompt_attr			prompt_attr;

	const xcommand_info_t *		cmd;

	uint16_t					argc;
	char * const *				argv;
} xcommand_callstack_t;

typedef struct xcommand_parse_buffer
{
	char						work_buff[XCOMMAND_LENGTH_MAX];
} xcommand_parse_buffer_t;

typedef struct xcommand_parse_result
{
	uint16_t					argc;
	char *						argv[XCOMMAND_ARG_NUM_MAX];
} xcommand_parse_result_t;


bool_t							xcommand_exec(enum xprompt_attr attr, const char *command);

bool_t							xcommand_parse(const char *command, xcommand_parse_buffer_t *buffer, xcommand_parse_result_t *result);

bool_t							xcommand_sub_call(xcommand_callstack_t *cs, const xcommand_info_t *sub_cmd_list, uint16_t sub_cmd_count);

bool_t							xcommand_arg_pop_text(xcommand_callstack_t *cs, const char **arg);
bool_t							xcommand_arg_pop(xcommand_callstack_t *cs, xcommand_arg_t *arg);

const char *					xcommand_text_line_get(const char *text, char *line_buffer, size_t line_buffer_size);

bool_t							xcommand_text2arg(const char *text, xcommand_arg_t *argv);
bool_t							xcommand_text2bin(const char *text, uint8_t *buffer, size_t buffer_size, size_t *result_size);
uint32_t						xcommand_text2uint(const char *text);

#endif /* XCOMMAND_H_ */
