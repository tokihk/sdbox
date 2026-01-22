/*
 * xprompt.h
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */

#ifndef XPROMPT_H_
#define XPROMPT_H_

#include "core/xstddef.h"


#define XPROMPT_LINE_TEXT_LEN		(96)

#define XPROMPT_RECORD_NUM			(24)

#define XPRINTF(...)				xprompt_printf(XPROMPT_ATTR_RECORD, __VA_ARGS__)

#define XPRINTF_DEBUG(...)			xprompt_printf(0, __VA_ARGS__)
#define XPRINTBIN_DEBUG(...)		xprompt_print_bin(0, __VA_ARGS__)


enum xprompt_attr
{
	XPROMPT_ATTR_SILENT				= (1 << 0),
	XPROMPT_ATTR_RECORD				= (1 << 1),
};

typedef struct xprompt_record
{
	char						text[XPROMPT_LINE_TEXT_LEN + 1];
} xprompt_record_t;


void							xprompt_init(void);
void							xprompt_poll(void);

void							xprompt_record_enumeration(bool_t (*callback)(uint16_t index, const xprompt_record_t *record, void *arg), void *arg);

void							xprompt_printf(enum xprompt_attr attr, const char *format, ... );
void							xprompt_print_bin(enum xprompt_attr attr, const uint8_t *data, size_t data_size);
void							xprompt_print_multiline(enum xprompt_attr attr, size_t indent, const char *text, size_t text_line_num);

void							xprompt_debug_printf(const char *format, ... );
void							xprompt_debug_print_bin(const uint8_t *data, size_t data_size);


#endif /* XPROMPT_H_ */
