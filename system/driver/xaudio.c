/*
 * xaudio.c
 *
 *  Created on: 2023/01/12
 *      Author: Hitoshi Kono
 */

#include "xaudio.h"

#include "driver/xremcon.h"

#include "prompt/xprompt.h"


typedef struct xaudio_open_device_info
{
	uint32_t					pcm_out_sampling_rate;
	enum xpcm_data_format		pcm_out_format;
	uint8_t						pcm_out_ch;
} xaudio_open_device_info_t;

/* ----------------------------------------------
 * Macros for defining device lists
 * ------------------------------------------- */
typedef struct xaudio_device
{
	bool_t						(*api_open)(const xaudio_config_t *config, xaudio_open_device_info_t *odi);
	void						(*api_close)(void);
	void						(*api_mute_set)(bool_t mute_off);
	size_t						(*api_pcm_data_add)(const uint8_t *data, size_t data_size);
} xaudio_device_t;

#define XAUDIO_DEVICE_LIST_BEGIN()				static const xaudio_device_t XAUDIO_DEVICE_LIST[] = {
#define XAUDIO_DEVICE_LIST_END()				};
#define XAUDIO_DEVICE_LIST_ITEM(prefix)			\
{												\
	.api_open=prefix##_open,					\
	.api_close=prefix##_close,					\
	.api_mute_set=prefix##_mute_set,			\
	.api_pcm_data_add=prefix##_pcm_data_add,	\
}

/* ----------------------------------------------
 * Import board method
 * ------------------------------------------- */
#if XBOARD_AUDIO_ENABLE
  #include "driver/xaudio_board.h"

  #define XBOARD_AUDIO_NUM		(XCOUNTOF(XAUDIO_DEVICE_LIST))

#else
  #define XBOARD_AUDIO_NUM		(0)

#endif


typedef struct xaudio_object
{
	xaudio_open_device_info_t	odi;
	bool_t						is_opened;
} xaudio_object_t;

static struct
{
	xaudio_object_t				objs[XBOARD_AUDIO_NUM];
} g_xaudio;


void xaudio_init(void)
{
#if XBOARD_AUDIO_ENABLE
	xaudio_init_board();
#endif
}

void xaudio_deinit(void)
{
#if XBOARD_AUDIO_ENABLE
	xaudio_deinit_board();
#endif
}

uint16_t xaudio_device_num_get(void)
{
	return (XBOARD_AUDIO_NUM);
}

bool_t xaudio_open(uint16_t id, const xaudio_config_t *config)
{
	bool_t is_opened = FALSE;

#if XBOARD_AUDIO_ENABLE
	if (   (id < XBOARD_AUDIO_NUM)
		&& (config != NULL)
	) {
		xaudio_object_t *obj = &g_xaudio.objs[id];

		if (!obj->is_opened)
		{
			memset(obj, 0, sizeof(*obj));

			obj->is_opened =  (XAUDIO_DEVICE_LIST[id].api_open)(config, &obj->odi);
		}

		is_opened = obj->is_opened;
	}
#endif

	return (is_opened);
}

void xaudio_close(uint16_t id)
{
#if XBOARD_AUDIO_ENABLE
	if (id < XBOARD_AUDIO_NUM)
	{
		xaudio_object_t *obj = &g_xaudio.objs[id];

		if (obj->is_opened)
		{
			(XAUDIO_DEVICE_LIST[id].api_close)();

			obj->is_opened = FALSE;
		}
	}
#endif
}

void xaudio_mute_set(uint16_t id, bool_t mute_off)
{
#if XBOARD_AUDIO_ENABLE
	if (id < XBOARD_AUDIO_NUM)
	{
		(XAUDIO_DEVICE_LIST[id].api_mute_set)(mute_off);
	}
#endif
}

size_t xaudio_pcm_data_add(uint16_t id, const uint8_t *data, size_t data_size)
{
	size_t add_size = 0;

#if XBOARD_AUDIO_ENABLE
	if ((id < XBOARD_AUDIO_NUM) && (data != NULL) && (data_size > 0))
	{
		add_size = (XAUDIO_DEVICE_LIST[id].api_pcm_data_add)(data, data_size);
	}
#endif

	return (add_size);
}
