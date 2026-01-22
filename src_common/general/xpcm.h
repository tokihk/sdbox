/*
 * xpcm.h
 *
 *  Created on: 2024/03/19
 *      Author: Hitoshi Kono
 */

#ifndef XPCM_H_
#define XPCM_H_

#include "core/xstddef.h"


enum xpcm_data_format
{
	XPCM_DATA_FORMAT_UNKNOWN,
	XPCM_DATA_FORMAT_U8,
	XPCM_DATA_FORMAT_S8,
	XPCM_DATA_FORMAT_U16BE,
	XPCM_DATA_FORMAT_U16LE,
	XPCM_DATA_FORMAT_S16BE,
	XPCM_DATA_FORMAT_S16LE,
	XPCM_DATA_FORMAT_U24BE,
	XPCM_DATA_FORMAT_U24LE,
	XPCM_DATA_FORMAT_S24BE,
	XPCM_DATA_FORMAT_S24LE,
	XPCM_DATA_FORMAT_U32BE,
	XPCM_DATA_FORMAT_U32LE,
	XPCM_DATA_FORMAT_S32BE,
	XPCM_DATA_FORMAT_S32LE,
};


#define XPCM_DATA_SIZE_MAX		(4)

#define XPCM_DATA_SIZE_GET(rate,bytes,ch,msec)		((uint32_t)(rate) / 1000ul * (bytes) * (ch) * (msec))


typedef struct xpcm_format_info
{
	uint8_t						size;
	bool_t						is_signed;
	bool_t						is_little;
} xpcm_format_info_t;

typedef struct xpcm_format_converter
{
	uint8_t						in_data[XPCM_DATA_SIZE_MAX];
	uint8_t						in_data_size;

	const xpcm_format_info_t *	in_format;
	const xpcm_format_info_t *	out_format;
} xpcm_format_converter_t;


void							xpcm_format_converter_init(xpcm_format_converter_t *obj, enum xpcm_data_format in_format, enum xpcm_data_format out_format);

bool_t							xpcm_format_converter_input(xpcm_format_converter_t *obj, uint8_t in_data, uint8_t *out_buffer, size_t out_buffer_size);

const xpcm_format_info_t *		xpcm_format_info_get(enum xpcm_data_format format);
enum xpcm_data_format			xpcm_format_type_get(const xpcm_format_info_t *info);
enum xpcm_data_format			xpcm_format_type_get_from_text(const char *text);


#endif /* XPCM_H_ */
