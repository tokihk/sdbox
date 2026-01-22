/*
 * xgate_speaker.c
 *
 *  Created on: 2024/05/05
 *      Author: Hitoshi Kono
 */

#include <config/xconfig_const.h>
#include "xgate_speaker.h"

#include "prompt/xprompt.h"
#include "prompt/xcommand.h"

#include "core/xmalloc.h"
#include "core/xtask.h"

#include "driver/xaudio.h"

#include "general/xpcm.h"

/* for Debug */
#include "driver/xgpio.h"
#include "driver/xremcon.h"


#undef  XGATE_CLASS_MOD_NAME
#define XGATE_CLASS_MOD_NAME speaker


#define XGATE_SPEAKER_DEVICE_ID_DEFAULT			(0)

#define XGATE_SPEAKER_SAMPLING_RATE_MIN			(100)
#define XGATE_SPEAKER_SAMPLING_RATE_MAX			(48000)
#define XGATE_SPEAKER_SAMPLING_RATE_DEFAULT		(16000)

#define XGATE_SPEAKER_PCM_FORMAT_DEFAULT		(XPCM_DATA_FORMAT_U16BE)

#define XGATE_SPEAKER_CHANNEL_MIN				(1)
#define XGATE_SPEAKER_CHANNEL_MAX				(2)
#define XGATE_SPEAKER_CHANNEL_DEFAULT			(1)

#define XGATE_SPEAKER_VOLUME_MIN				(0)
#define XGATE_SPEAKER_VOLUME_MAX				(255)
//#define XGATE_SPEAKER_VOLUME_DEFAULT			(128)
#define XGATE_SPEAKER_VOLUME_DEFAULT			(200)

#define XGATE_SPEAKER_play_BUFFER_SIZE_DEFAULT	(3200)		/* 16bit * 16k * 2ch * 50ms */


XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME)
{
	uint16_t					audio_device_id;
	uint32_t					sampling_rate;
	enum xpcm_data_format		pcm_format;
	uint8_t						channel;
	uint8_t						volume;
	size_t						play_buffer_size;
};

XGATE_CLASS_OBJECT(XGATE_CLASS_MOD_NAME)
{
	uint8_t *					play_buffer;
	size_t						play_buffer_in;
	size_t						play_buffer_out;
	size_t						play_data_count;

	bool_t						is_playing;
};


const char XGATE_SPEAKER_NAME[] = "speaker";
const char XGATE_SPEAKER_HELP[] = \
		"Speaker.\n" \
		;


static void xgate_speaker_play_data_enqueue(
		XGATE_CLASS_OBJECT(XGATE_CLASS_MOD_NAME) *gobjc,
		XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc,
		const uint8_t *data,
		size_t data_size
) {
	if ((gobjc->play_data_count + data_size) < gconfc->play_buffer_size)
	{
		size_t copy_size = 0;

		while (data_size > 0)
		{
			copy_size = gconfc->play_buffer_size - gobjc->play_buffer_in;
			if (copy_size > data_size)
			{
				copy_size = data_size;
			}

			memcpy(gobjc->play_buffer + gobjc->play_buffer_in, data, copy_size);

			data_size -= copy_size;
			gobjc->play_data_count += copy_size;

			gobjc->play_buffer_in += copy_size;
			if (gobjc->play_buffer_in >= gconfc->play_buffer_size)
			{
				gobjc->play_buffer_in = 0;
			}
		}
	}
}

static void xgate_speaker_play_data_setup(
		XGATE_CLASS_OBJECT(XGATE_CLASS_MOD_NAME) *gobjc,
		XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc
) {
	size_t setup_size;

	/* ある程度のデータが溜まっていれば出力開始 */
	if (   (!gobjc->is_playing)
		&& (gobjc->play_data_count >= gconfc->play_buffer_size / 2)
	) {
		gobjc->is_playing = TRUE;
	}

	if (gobjc->is_playing)
	{
		while (gobjc->play_data_count > 0)
		{
			/* 設定サイズを取得 */
			if (gobjc->play_buffer_out > gobjc->play_buffer_in)
			{
				setup_size = gconfc->play_buffer_size - gobjc->play_buffer_out;
			}
			else
			{
				setup_size = gobjc->play_buffer_in - gobjc->play_buffer_out;
			}

			/* デバイスへ再生データを設定 */
			setup_size = xaudio_pcm_data_add(gconfc->audio_device_id, gobjc->play_buffer + gobjc->play_buffer_out, setup_size);

			/* 1バイトも設定できなかった場合は終了 */
			if (setup_size == 0)
			{
				break;
			}

			gobjc->play_data_count -= setup_size;

			if ((gobjc->play_buffer_out + setup_size) < gconfc->play_buffer_size)
			{
				gobjc->play_buffer_out += setup_size;
			}
			else
			{
				gobjc->play_buffer_out = gobjc->play_buffer_out + setup_size - gconfc->play_buffer_size;
			}
		}
	}
}

static void xgate_speaker_config_init(xgate_class_config_t *gconf)
{
	XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc = (XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *)gconf->custom;

	gconfc->audio_device_id  = XGATE_SPEAKER_DEVICE_ID_DEFAULT;

	gconfc->sampling_rate = XGATE_SPEAKER_SAMPLING_RATE_DEFAULT;
	gconfc->pcm_format    = XGATE_SPEAKER_PCM_FORMAT_DEFAULT;
	gconfc->channel       = XGATE_SPEAKER_CHANNEL_DEFAULT;

	gconfc->volume           = XGATE_SPEAKER_VOLUME_DEFAULT;
	gconfc->play_buffer_size = XGATE_SPEAKER_play_BUFFER_SIZE_DEFAULT;
}

static void xgate_speaker_config_deinit(xgate_class_config_t *gconf)
{
}

static bool_t xgate_speaker_config_setup(xgate_class_config_t *gconf, const xcommand_arg_t *arg)
{
	XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc = (XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *)gconf->custom;
	bool_t update = FALSE;

	/* sampling_rate */
	if (strcmp(arg->name, "rate") == 0)
	{
		gconfc->sampling_rate = (uint32_t)atol(arg->value);
		gconfc->sampling_rate = XMIN(gconfc->sampling_rate, XGATE_SPEAKER_SAMPLING_RATE_MAX);
		gconfc->sampling_rate = XMAX(gconfc->sampling_rate, XGATE_SPEAKER_SAMPLING_RATE_MIN);

		update = TRUE;
	}

	/* format */
	else if (strcmp(arg->name, "format") == 0)
	{
		gconfc->pcm_format = xpcm_format_type_get_from_text(arg->value);

		update = TRUE;
	}

	/* channel */
	else if (strcmp(arg->name, "channel") == 0)
	{
		gconfc->channel = (uint8_t)atoi(arg->value);
		gconfc->channel = XMIN(gconfc->channel, XGATE_SPEAKER_CHANNEL_MAX);
		gconfc->channel = XMAX(gconfc->channel, XGATE_SPEAKER_CHANNEL_MIN);

		update = TRUE;
	}

	/* volume */
	else if (strcmp(arg->name, "volume") == 0)
	{
		gconfc->volume = (uint8_t)atoi(arg->value);
		gconfc->volume = XMIN(gconfc->volume, XGATE_SPEAKER_VOLUME_MAX);
		gconfc->volume = XMAX(gconfc->volume, XGATE_SPEAKER_VOLUME_MIN);

		update = TRUE;
	}

	/* buffer_size */
	else if (strcmp(arg->name, "buffer_size") == 0)
	{
		gconfc->play_buffer_size = (size_t)atoi(arg->value);
		update = TRUE;
	}

	return (update);
}

static void xgate_speaker_config_print(xgate_class_config_t *gconf, enum xprompt_attr attr)
{
	XGATE_CLASS_CONFIG(mod_name) *gconfc = (XGATE_CLASS_CONFIG(mod_name) *)gconf->custom;

}

static bool_t xgate_speaker_open(xgate_object_t *gobj)
{
	XGATE_CLASS_OBJECT_VAR(gobj, XGATE_CLASS_MOD_NAME, gobjc);
	XGATE_CLASS_CONFIG_VAR(gobj, XGATE_CLASS_MOD_NAME, gconfc);

	bool_t			error = FALSE;
	xaudio_config_t	audio_conf = {0};

	audio_conf.sampling_rate = gconfc->sampling_rate;
	audio_conf.pcm_format = gconfc->pcm_format;
	audio_conf.channel = gconfc->channel;
	audio_conf.volume = gconfc->volume;

	gobjc->play_buffer_in = 0;
	gobjc->play_buffer_out = 0;
	gobjc->play_data_count = 0;

	/* 再生データ用バッファ確保 */
	if ((gobjc->play_buffer = xmalloc(gconfc->play_buffer_size)) == NULL)
	{
		error = TRUE;
	}

	/* デバイス初期化 */
	else if (!xaudio_open(gconfc->audio_device_id, &audio_conf))
	{
		error = TRUE;
	}

	/* Success */
	else
	{
	}

	return (!error);
}

static bool_t xgate_speaker_close(xgate_object_t *gobj)
{
	XGATE_CLASS_OBJECT_VAR(gobj, XGATE_CLASS_MOD_NAME, gobjc);
	XGATE_CLASS_CONFIG_VAR(gobj, XGATE_CLASS_MOD_NAME, gconfc);

	if (gobjc != NULL) {
		/* デバイス停止 */
		xaudio_close(gconfc->audio_device_id);

		/* 再生バッファを解放 */
		if (gobjc->play_buffer != NULL)
		{
			xfree(gobjc->play_buffer);
			gobjc->play_buffer = NULL;
		}

	}

	return (TRUE);
}

static void xgate_speaker_poll(xgate_object_t *gobj)
{
	XGATE_CLASS_OBJECT_VAR(gobj, XGATE_CLASS_MOD_NAME, gobjc);
	XGATE_CLASS_CONFIG_VAR(gobj, XGATE_CLASS_MOD_NAME, gconfc);

	xgate_speaker_play_data_setup(gobjc, gconfc);
}

static void xgate_speaker_send(xgate_object_t *gobj, const uint8_t *data, size_t data_size)
{
	XGATE_CLASS_OBJECT_VAR(gobj, XGATE_CLASS_MOD_NAME, gobjc);
	XGATE_CLASS_CONFIG_VAR(gobj, XGATE_CLASS_MOD_NAME, gconfc);

	xgate_speaker_play_data_enqueue(gobjc, gconfc, data, data_size);
}

XGATE_CLASS_DEFINE(XGATE_CLASS_MOD_NAME, XGATE_SPEAKER_NAME, XGATE_SPEAKER_HELP)

