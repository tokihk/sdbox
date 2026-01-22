/*
 * xwavegen.h
 *
 *  Created on: 2024/03/21
 *      Author: Hitoshi Kono
 */

#ifndef XWAVEGEN_H_
#define XWAVEGEN_H_

#include "core/xstddef.h"

#include "general/xpcm.h"


enum xwave_form_type
{
	XWAVE_FORM_UNKNOWN,
	XWAVE_FORM_SINE,
};


typedef struct xwave_gen_config
{
	uint32_t					wave_freq;
	uint32_t					sampling_rate;
	enum xpcm_data_format		pcm_format;
	uint16_t					amplification;			/* 0-10000 */
} xwave_gen_config_t;


#define XWAVE_ONE_CYCLE_DATA_SIZE(wf,sf,bps)	((sf) / (wf) * (bps))


const char *					xwave_form_name_get(enum xwave_form_type type);
enum xwave_form_type			xwave_form_type_get(const char *name);

void							xwave_generate(uint8_t *buffer, size_t buffer_size, enum xwave_form_type form_type, const xwave_gen_config_t *config);

size_t							xwave_ch_copy(
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
								);


#endif /* XWAVEGEN_H_ */
