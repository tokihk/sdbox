/*
 * xwavegen.c
 *
 *  Created on: 2024/03/21
 *      Author: Hitoshi Kono
 */

#include "xwavegen.h"

#include <math.h>
#include <string.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


typedef struct xwave_form_info
{
	enum xwave_form_type	type;
	const char *			name;
} xwave_form_info_t;

const xwave_form_info_t XWAVE_FORM_LIST[] =
{
	{ .type = XWAVE_FORM_SINE, .name = "sine" },
};


static uint32_t xwave_pcm_value_get_sine(uint32_t cycle_sample_count, uint32_t sample_offset)
{
	return (uint32_t)((sin(M_PI * 2 / cycle_sample_count * sample_offset) + 1) * 0x7FFFFFFF);
}

static void xwave_pcm_value_serialize(uint32_t value, uint8_t *buffer, const xpcm_format_info_t *format_info)
{
	uint8_t out_byte_size = 0;

	/* ビット精度調整 */
	value >>= 8 * (4 - format_info->size);

	/* 符号調整 */
	if (format_info->is_signed)
	{
		value ^= 1u << (8 * format_info->size - 1);
	}

	/* 出力 */
	if (format_info->is_little)
	{
		/* リトルエンディアン */
		for (out_byte_size = 0; out_byte_size < format_info->size; out_byte_size++)
		{
			buffer[out_byte_size] = (uint8_t)(value & 0xFF);
			value >>= 8;
		}
	}
	else
	{
		/* ビックエンディアン */

		for (out_byte_size = 0; out_byte_size < format_info->size; out_byte_size++)
		{
			buffer[out_byte_size] = (uint8_t)(value >> (8 * (format_info->size - out_byte_size - 1)));
		}
	}
}

const char *xwave_form_name_get(enum xwave_form_type type)
{
	const char *name = "unknown";
	uint8_t index;

	for (index = 0; index < XCOUNTOF(XWAVE_FORM_LIST); index++)
	{
		if (XWAVE_FORM_LIST[index].type == type)
		{
			name = XWAVE_FORM_LIST[index].name;
			break;
		}
	}

	return (name);
}

enum xwave_form_type xwave_form_type_get(const char *name)
{
	enum xwave_form_type type = XWAVE_FORM_UNKNOWN;
	uint8_t index;

	for (index = 0; index < XCOUNTOF(XWAVE_FORM_LIST); index++)
	{
		if (strcmp(XWAVE_FORM_LIST[index].name, name) == 0)
		{
			type = XWAVE_FORM_LIST[index].type;
			break;
		}
	}

	return (type);
}

void xwave_generate(uint8_t *buffer, size_t buffer_size, enum xwave_form_type form_type, const xwave_gen_config_t *config)
{
	if (   (buffer != NULL)
		&& (buffer_size > 0)
		&& (config != NULL)
	) {
		uint32_t (*pcm_value_get)(uint32_t cycle_sample_count, uint32_t sample_offset);
		const xpcm_format_info_t *pcm_format_info;

		/* 波形タイプ毎の出力関数取得 */
		switch (form_type)
		{
			case XWAVE_FORM_SINE:	pcm_value_get = xwave_pcm_value_get_sine;	break;
			default:				pcm_value_get = NULL;						break;
		}

		/* サンプルのフォーマット情報取得 */
		pcm_format_info = xpcm_format_info_get(config->pcm_format);

		if (   (pcm_value_get != NULL)
			&& (pcm_format_info != NULL)
		) {
			uint32_t					cycle_sample_count;
			uint32_t					cycle_sample_offset = 0;
			uint32_t					pcm_value;

			/* 波形の1サイクルのサンプル数算出 */
			cycle_sample_count = config->sampling_rate / config->wave_freq;

			/* 出力バッファが満タンになるまで繰り返す */
			while (buffer_size >= pcm_format_info->size)
			{
				/* 1サンプルデータ取得 */
				pcm_value = (pcm_value_get)(cycle_sample_count, cycle_sample_offset);

				/* バッファへ出力 */
				xwave_pcm_value_serialize(pcm_value, buffer, pcm_format_info);

				cycle_sample_offset++;
				cycle_sample_offset %= cycle_sample_count;

				buffer += pcm_format_info->size;
				buffer_size -= pcm_format_info->size;
			}
		}
	}
}

size_t xwave_ch_copy(
	uint8_t *		dst_buffer,
	size_t			dst_buffer_size,
	uint8_t			dst_ch,
	uint8_t			dst_ch_max,
	const uint8_t *	src_data,
	size_t			src_data_size,
	uint8_t			src_ch,
	uint8_t			src_ch_max,
	size_t			src_data_offset,
	uint8_t			byte_per_sample
) {
	if (   (dst_buffer != NULL)
		&& (dst_buffer_size > 0)
		&& (src_data != NULL)
		&& (src_data_size > 0)
	) {
		uint8_t *		dst_ch_ptr;
		const uint8_t *	src_ch_ptr;
		size_t			dst_data_offset = 0;
		uint8_t			byte_index;
		uint8_t			dst_ch_set_offset;
		uint8_t			src_ch_set_offset;
		uint8_t			dst_ch_shift_offset;
		uint8_t			src_ch_shift_offset;

		dst_ch_set_offset = dst_ch * byte_per_sample;
		src_ch_set_offset = src_ch * byte_per_sample;
		dst_ch_shift_offset = dst_ch_max * byte_per_sample;
		src_ch_shift_offset = src_ch_max * byte_per_sample;

		/* バッファを超えない位置に調整 */
		src_data_offset %= src_data_size;

		/* ブロック境界に移動 */
		src_data_offset -= src_data_offset % byte_per_sample;

		while ((dst_data_offset + byte_per_sample) <= dst_buffer_size)
		{
			/* CH位置にポインタを移動 */
			dst_ch_ptr = dst_buffer + dst_data_offset + dst_ch_set_offset;
			src_ch_ptr = src_data + src_data_offset + src_ch_set_offset;

			/* 1サンプルをコピー */
			for (byte_index = 0; byte_index < byte_per_sample; byte_index++)
			{
				*dst_ch_ptr++ = *src_ch_ptr++;
			}

			dst_data_offset += dst_ch_shift_offset;
			src_data_offset += src_ch_shift_offset;

			if (src_data_offset >= src_data_size)
			{
				src_data_offset -= src_data_size;
				src_data_offset -= src_data_offset % byte_per_sample;
			}
		}
	}

	return (src_data_offset);
}

