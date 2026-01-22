/*
 * xpcm.c
 *
 *  Created on: 2024/03/19
 *      Author: Hitoshi Kono
 */

#include "xpcm.h"

#include <string.h>


typedef struct xpcm_format_table_data
{
	enum xpcm_data_format		format;
	const char *				name;
	xpcm_format_info_t			info;
} xpcm_format_table_data_t;


static const xpcm_format_table_data_t XPCM_FORMAT_INFO_TABLE[] =
{
	{ .format = XPCM_DATA_FORMAT_U8,     .name = "u8",    .info = { .size = 1, .is_signed = FALSE, .is_little = FALSE } },
	{ .format = XPCM_DATA_FORMAT_S8,     .name = "s8",    .info = { .size = 1, .is_signed = TRUE,  .is_little = FALSE } },
	{ .format = XPCM_DATA_FORMAT_U16BE,  .name = "u16be", .info = { .size = 2, .is_signed = FALSE, .is_little = FALSE } },
	{ .format = XPCM_DATA_FORMAT_U16LE,  .name = "u16le", .info = { .size = 2, .is_signed = FALSE, .is_little = TRUE  } },
	{ .format = XPCM_DATA_FORMAT_S16BE,  .name = "s16be", .info = { .size = 2, .is_signed = TRUE,  .is_little = FALSE } },
	{ .format = XPCM_DATA_FORMAT_S16LE,  .name = "s16le", .info = { .size = 2, .is_signed = TRUE,  .is_little = TRUE  } },
	{ .format = XPCM_DATA_FORMAT_U24BE,  .name = "u24be", .info = { .size = 3, .is_signed = FALSE, .is_little = FALSE } },
	{ .format = XPCM_DATA_FORMAT_U24LE,  .name = "u24le", .info = { .size = 3, .is_signed = FALSE, .is_little = TRUE  } },
	{ .format = XPCM_DATA_FORMAT_S24BE,  .name = "s24be", .info = { .size = 3, .is_signed = TRUE,  .is_little = FALSE } },
	{ .format = XPCM_DATA_FORMAT_S24LE,  .name = "s24le", .info = { .size = 3, .is_signed = TRUE,  .is_little = TRUE  } },
	{ .format = XPCM_DATA_FORMAT_U32BE,  .name = "u32be", .info = { .size = 4, .is_signed = FALSE, .is_little = FALSE } },
	{ .format = XPCM_DATA_FORMAT_U32LE,  .name = "u32le", .info = { .size = 4, .is_signed = FALSE, .is_little = TRUE  } },
	{ .format = XPCM_DATA_FORMAT_S32BE,  .name = "s32be", .info = { .size = 4, .is_signed = TRUE,  .is_little = FALSE } },
	{ .format = XPCM_DATA_FORMAT_S32LE,  .name = "s32le", .info = { .size = 4, .is_signed = TRUE,  .is_little = TRUE  } },
};


static uint32_t xpcm_binary_to_value(const uint8_t *in_data, uint8_t in_data_size, const xpcm_format_info_t *in_format_info)
{
	uint32_t value = 0;
	uint8_t  data_offset;

	/* Little endian */
	if (in_format_info->is_little)
	{
		for (data_offset = 0; data_offset < in_format_info->size; data_offset++) {
			value <<= 8;
			value += in_data[in_format_info->size - data_offset - 1];
		}
	}

	/* Big endian */
	else
	{
		for (data_offset = 0; data_offset < in_format_info->size; data_offset++) {
			value <<= 8;
			value += in_data[data_offset];
		}
	}

	return (value);
}

static void xpcm_value_to_binary(uint32_t value, uint8_t *out_buffer, const xpcm_format_info_t *out_format_info)
{
	uint8_t  data_offset;

	/* Little endian */
	if (out_format_info->is_little)
	{
		for (data_offset = 0; data_offset < out_format_info->size; data_offset++) {
			out_buffer[data_offset] = value & 0xFF;
			value >>= 8;
		}
	}

	/* Big endian */
	else
	{
		for (data_offset = 0; data_offset < out_format_info->size; data_offset++) {
			out_buffer[out_format_info->size - data_offset - 1] = value & 0xFF;
			value >>= 8;
		}
	}
}

static uint32_t xpcm_value_format_convert(uint32_t value, const xpcm_format_info_t *in_format, const xpcm_format_info_t *out_format)
{
	uint8_t in_format_bit_size;
	uint8_t out_format_bit_size;
	bool_t  is_signed = FALSE;

	in_format_bit_size  = in_format->size  << 3;
	out_format_bit_size = out_format->size << 3;

	/* 符号分解 */
	if (in_format->is_signed)
	{
		if ((value >> (in_format_bit_size - 1)) != 0) {
			is_signed = TRUE;
			value &= (0xFFFFFFFFu >> (32 - in_format_bit_size + 1));
		}
	}
	else
	{
		if ((value >> (in_format_bit_size - 1)) == 0) {
			is_signed = TRUE;
		}
	}

	/* amplifier */
	if (in_format_bit_size < out_format_bit_size)
	{
		value <<= (out_format_bit_size - in_format_bit_size);
	}
	else if (in_format_bit_size > out_format_bit_size)
	{
		value >>= (in_format_bit_size - out_format_bit_size);
	}

	/* 符号復元 */
	if (out_format->is_signed)
	{
		if (is_signed)
		{
			value |= (1u << (out_format_bit_size - 1));
		}
		else
		{
			value &= (0xFFFFFFFFu >> (32 - out_format_bit_size + 1));
		}
	}
	else
	{
		if (!is_signed)
		{
			value |= (1u << (out_format_bit_size - 1));
		}
	}

	return (value);
}

void xpcm_format_convert_init(xpcm_format_converter_t *obj, enum xpcm_data_format in_format, enum xpcm_data_format out_format)
{
	if (obj != NULL)
	{
		obj->in_data_size = 0;

		obj->in_format = xpcm_format_info_get(in_format);
		obj->out_format = xpcm_format_info_get(out_format);
	}
}

bool_t xpcm_format_convert(xpcm_format_converter_t *obj, uint8_t in_data, uint8_t *out_buffer, size_t out_buffer_size)
{
	bool_t		output_ok = FALSE;
	uint32_t	value;

	/* 入力フォーマットサイズまでデータ収集 */
	if (obj->in_data_size < obj->in_format->size)
	{
		obj->in_data[obj->in_data_size++] = in_data;
	}

	/* 入力フォーマットサイズのデータ収集完了 */
	if (obj->in_data_size >= obj->in_format->size)
	{
		if (out_buffer_size >= obj->out_format->size)
		{
			/* 入力バッファを整数値に変換 */
			value = xpcm_binary_to_value(obj->in_data, obj->in_data_size, obj->in_format);

			/* 出力フォーマットに合わせて値調整 */
			value = xpcm_value_format_convert(value, obj->in_format, obj->out_format);

			/* 整数値を出力バッファに変換 */
			xpcm_value_to_binary(value, out_buffer, obj->out_format);

			output_ok = TRUE;
		}

		obj->in_data_size = 0;
	}

	return (output_ok);
}

const xpcm_format_info_t *xpcm_format_info_get(enum xpcm_data_format format)
{
	const xpcm_format_info_t *info = NULL;
	uint8_t table_index;

	for (table_index = 0; table_index < XCOUNTOF(XPCM_FORMAT_INFO_TABLE); table_index++)
	{
		if (XPCM_FORMAT_INFO_TABLE[table_index].format == format)
		{
			info = &(XPCM_FORMAT_INFO_TABLE[table_index].info);
			break;
		}
	}

	return (info);
}

enum xpcm_data_format xpcm_format_type_get(const xpcm_format_info_t *info)
{
	enum xpcm_data_format format = XPCM_DATA_FORMAT_UNKNOWN;

	if (info != NULL)
	{
		uint8_t table_index;

		for (table_index = 0; table_index < XCOUNTOF(XPCM_FORMAT_INFO_TABLE); table_index++)
		{
			if (   (XPCM_FORMAT_INFO_TABLE[table_index].info.size == info->size)
				&& (XPCM_FORMAT_INFO_TABLE[table_index].info.is_little == info->is_little)
				&& (XPCM_FORMAT_INFO_TABLE[table_index].info.is_signed == info->is_signed)
			) {
				format = XPCM_FORMAT_INFO_TABLE[table_index].format;
				break;
			}
		}

	}

	return (format);
}

enum xpcm_data_format xpcm_format_type_get_from_text(const char *text)
{
	enum xpcm_data_format format = XPCM_DATA_FORMAT_UNKNOWN;

	if (text != NULL)
	{
		uint8_t table_index;

		for (table_index = 0; table_index < XCOUNTOF(XPCM_FORMAT_INFO_TABLE); table_index++)
		{
			if (strcmp(XPCM_FORMAT_INFO_TABLE[table_index].name, text) == 0)
			{
				format = XPCM_FORMAT_INFO_TABLE[table_index].format;
				break;
			}
		}
	}

	return (format);
}
