/*
 * xsound.h
 *
 *  Created on: 2023/01/12
 *      Author: Hitoshi Kono
 */

#ifndef XAUDIO_H_
#define XAUDIO_H_

#include "core/xstddef.h"

#include "general/xpcm.h"


typedef struct xaudio_config
{
	uint32_t					sampling_rate;
	enum xpcm_data_format		pcm_format;
	uint8_t						channel;
	uint8_t						volume;
} xaudio_config_t;


void							xaudio_init(void);
void							xaudio_deinit(void);

uint16_t						xaudio_device_num_get(void);

bool_t							xaudio_open(uint16_t id, const xaudio_config_t *config);
void							xaudio_close(uint16_t id);

void							xaudio_mute_set(uint16_t id, bool_t mute_off);

size_t							xaudio_pcm_data_add(uint16_t id, const uint8_t *data, size_t data_size);


#endif /* XAUDIO_H_ */
