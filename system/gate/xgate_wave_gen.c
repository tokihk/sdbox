/*
 * xgate_wave_gen.c
 *
 *  Created on: 2024/05/05
 *      Author: Hitoshi Kono
 */

#include <config/xconfig_const.h>
#include "xgate_wave_gen.h"

#include "prompt/xprompt.h"
#include "prompt/xcommand.h"

#include "core/xmalloc.h"
#include "core/xtask.h"

#include "general/xwavegen.h"


#undef  XGATE_CLASS_MOD_NAME
#define XGATE_CLASS_MOD_NAME	wave_gen


#define XGATE_WAVE_GEN_SETUP_IVAL				(20)

#define XGATE_WAVE_GEN_WAVE_FORM_DEFAULT		(XWAVE_FORM_SINE)

#define XGATE_WAVE_GEN_WAVE_FREQ_MIN			(40)
#define XGATE_WAVE_GEN_WAVE_FREQ_MAX			(16000)
#define XGATE_WAVE_GEN_WAVE_FREQ_DEFAULT		(1000)

#define XGATE_WAVE_GEN_AMP_MIN					(0)
#define XGATE_WAVE_GEN_AMP_MAX					(10000)
#define XGATE_WAVE_GEN_AMP_DEFAULT				(10000)
#define XGATE_WAVE_GEN_AMP_SET(va,vs)			do { (va) = XMAX(XGATE_WAVE_GEN_AMP_MIN,XMIN((vs),XGATE_WAVE_GEN_AMP_MAX)); } while (0)

#define XGATE_WAVE_GEN_SAMPLING_RATE_MIN		(8000)
#define XGATE_WAVE_GEN_SAMPLING_RATE_MAX		(48000)
#define XGATE_WAVE_GEN_SAMPLING_RATE_DEFAULT	(16000)

#define XGATE_WAVE_GEN_PCM_FORMAT_DEFAULT		(XPCM_DATA_FORMAT_U16BE)

#define XGATE_WAVE_GEN_CHANNEL_MIN				(1)
#define XGATE_WAVE_GEN_CHANNEL_MAX				(2)
#define XGATE_WAVE_GEN_CHANNEL_DEFAULT			(1)


typedef struct xgate_wave_gen_ch_config
{
	enum xwave_form_type		form_type;
	uint32_t					wave_freq;
	uint16_t					amplification;			/* 0-10000 */
} xgate_wave_gen_ch_config_t;

typedef struct xgate_wave_buffer
{
	uint8_t *					data;
	size_t						data_size;
} xgate_wave_buffer_t;

typedef struct xgate_wave_ch_data
{
	xgate_wave_buffer_t			wave_buffer;
	size_t						load_offset;
} xgate_wave_ch_data_t;

XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME)
{
	xgate_wave_gen_ch_config_t	ch_conf[XGATE_WAVE_GEN_CHANNEL_MAX];

	uint32_t					sampling_rate;
	enum xpcm_data_format		pcm_format;
	uint8_t						channel;
};

XGATE_CLASS_OBJECT(XGATE_CLASS_MOD_NAME)
{
	xtask_t						task_setup;

	const xpcm_format_info_t *	pcm_format_info;

	xgate_wave_buffer_t			output_buffer;
	xgate_wave_ch_data_t		ch_buffer[XGATE_WAVE_GEN_CHANNEL_MAX];

	uint32_t					setup_tick_prev;
};


const char XGATE_WAVE_GEN_NAME[] = "wave-gen";
const char XGATE_WAVE_GEN_HELP[] = \
		"Wave Generator.\n" \
		"  type   : [0-1] Wave type.\n"
		;


static void xgate_wave_gen_wave_buffer_free(xgate_wave_buffer_t *buffer)
{
	if (buffer != NULL)
	{
		if (buffer->data != NULL)
		{
			xfree(buffer->data);
			buffer->data = NULL;
		}
		buffer->data_size = 0;
	}
}

static bool_t xgate_wave_gen_wave_buffer_alloc(xgate_wave_buffer_t *buffer, size_t size)
{
	bool_t alloc_ok = FALSE;

	if (   (buffer != NULL)
		&& (size > 0)
	) {
		buffer->data = xmalloc(size);
		if (buffer->data != NULL)
		{
			buffer->data_size = size;
			alloc_ok = TRUE;
		}
	}

	return (alloc_ok);
}

static void xgate_wave_gen_ch_data_init
(
	XGATE_CLASS_OBJECT(XGATE_CLASS_MOD_NAME) *gobjc,
	XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc
)
{
	uint8_t ch_index;
	xgate_wave_ch_data_t *ch_data;

	for (ch_index = 0; ch_index < gconfc->channel; ch_index++)
	{
		ch_data = &gobjc->ch_buffer[ch_index];

		if (ch_data->wave_buffer.data != NULL)
		{
			xwave_gen_config_t wave_conf = {0};

			wave_conf.sampling_rate = gconfc->sampling_rate;
			wave_conf.pcm_format = gconfc->pcm_format;
			wave_conf.wave_freq = gconfc->ch_conf[ch_index].wave_freq;
			wave_conf.amplification = gconfc->ch_conf[ch_index].amplification;

			xwave_generate(
					ch_data->wave_buffer.data,
					ch_data->wave_buffer.data_size,
					gconfc->ch_conf[ch_index].form_type,
					&wave_conf
			);
		}

		ch_data->load_offset = 0;
	}
}

static void xgate_wave_gen_output_buffer_setup
(
	XGATE_CLASS_OBJECT(XGATE_CLASS_MOD_NAME) *gobjc,
	XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc
)
{
	if (gconfc->channel > 0)
	{
		uint8_t ch_index;

		for (ch_index = 0; ch_index < gconfc->channel; ch_index++)
		{
			gobjc->ch_buffer[ch_index].load_offset = xwave_ch_copy(
					gobjc->output_buffer.data,
					gobjc->output_buffer.data_size,
					ch_index,
					gconfc->channel,
					gobjc->ch_buffer[0].wave_buffer.data,
					gobjc->ch_buffer[0].wave_buffer.data_size,
					0,
					1,
					gobjc->ch_buffer[ch_index].load_offset,
					gobjc->pcm_format_info->size
			);
		}
	}
}

static void xgate_wave_gen_config_init(xgate_class_config_t *gconf)
{
	XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc = (XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *)gconf->custom;

	gconfc->pcm_format = XPCM_DATA_FORMAT_U16BE;
	gconfc->sampling_rate = XGATE_WAVE_GEN_SAMPLING_RATE_DEFAULT;
	gconfc->channel = XGATE_WAVE_GEN_CHANNEL_DEFAULT;

	gconfc->ch_conf[0].form_type = XGATE_WAVE_GEN_WAVE_FORM_DEFAULT;
	gconfc->ch_conf[0].wave_freq = XGATE_WAVE_GEN_WAVE_FREQ_DEFAULT;
	gconfc->ch_conf[0].amplification = XGATE_WAVE_GEN_AMP_DEFAULT;
}

static void xgate_wave_gen_config_deinit(xgate_class_config_t *gconf)
{
}

static bool_t xgate_wave_gen_config_setup(xgate_class_config_t *gconf, const xcommand_arg_t *arg)
{
	XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc = (XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *)gconf->custom;

	bool_t update = FALSE;

	/* form */
	if (strcmp(arg->name, "form") == 0)
	{
		uint32_t form;

		form = xwave_form_type_get(arg->value);
		if (form != XWAVE_FORM_UNKNOWN)
		{
			gconfc->ch_conf[0].form_type = form;
			update = TRUE;
		}
	}

	/* wave frequency */
	else if (strcmp(arg->name, "wfreq") == 0)
	{
		uint32_t wfreq;

		wfreq = (uint32_t)atoi(arg->value);
		if ((wfreq >= XGATE_WAVE_GEN_WAVE_FREQ_MIN) && (wfreq <= XGATE_WAVE_GEN_WAVE_FREQ_MAX))
		{
			gconfc->ch_conf[0].wave_freq = wfreq;
			update = TRUE;
		}
	}

	/* sampling frequency */
	else if (strcmp(arg->name, "rate") == 0)
	{
		gconfc->sampling_rate = (uint32_t)atol(arg->value);
		gconfc->sampling_rate = XMIN(gconfc->sampling_rate, XGATE_WAVE_GEN_SAMPLING_RATE_MAX);
		gconfc->sampling_rate = XMAX(gconfc->sampling_rate, XGATE_WAVE_GEN_SAMPLING_RATE_MIN);

		update = TRUE;
	}

	/* format */
	else if (strcmp(arg->name, "format") == 0)
	{
		gconfc->pcm_format = xpcm_format_type_get_from_text(arg->value);

		update = TRUE;
	}

	return (update);
}

static void xgate_wave_gen_config_print(xgate_class_config_t *gconf, enum xprompt_attr attr)
{
	XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *	gconfc = (XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *)gconf->custom;
	xgate_wave_gen_ch_config_t *				ch_config;
	uint8_t										ch_index;

	for (ch_index = 0; ch_index < gconfc->channel; ch_index++)
	{
		ch_config = &gconfc->ch_conf[ch_index];

		xprompt_printf(
			attr,
			"%s - %luHz %uch",
			xwave_form_name_get(ch_config->form_type),
			ch_config->wave_freq
		);
	}
}

static bool_t xgate_wave_gen_open(xgate_object_t *gobj)
{
	XGATE_CLASS_OBJECT_VAR(gobj, XGATE_CLASS_MOD_NAME, gobjc);
	XGATE_CLASS_CONFIG_VAR(gobj, XGATE_CLASS_MOD_NAME, gconfc);
	uint8_t ch_index;

	/* PCMフォーマット取得 */
	gobjc->pcm_format_info = xpcm_format_info_get(gconfc->pcm_format);
	if (gobjc->pcm_format_info == NULL)
	{
		return (FALSE);
	}

	/* CHバッファ確保 */
	for (ch_index = 0; (ch_index < XCOUNTOF(gobjc->ch_buffer)) && (ch_index < gconfc->channel); ch_index++)
	{
		if (!xgate_wave_gen_wave_buffer_alloc(
				&gobjc->ch_buffer[ch_index].wave_buffer,
				XWAVE_ONE_CYCLE_DATA_SIZE(
						gconfc->ch_conf[ch_index].wave_freq,
						gconfc->sampling_rate,
						gobjc->pcm_format_info->size
				))
		) {
			return (FALSE);
		}
	}

	/* 出力バッファ確保 */
	if (!xgate_wave_gen_wave_buffer_alloc(
			&gobjc->output_buffer,
			gconfc->sampling_rate * gobjc->pcm_format_info->size * gconfc->channel / 1000 * XGATE_WAVE_GEN_SETUP_IVAL)
	) {
		return (FALSE);
	}

	/* CH毎の波形データ生成 */
	xgate_wave_gen_ch_data_init(gobjc, gconfc);

	/* 初回データ読み込み */
	xgate_wave_gen_output_buffer_setup(gobjc, gconfc);

	return (TRUE);
}

static bool_t xgate_wave_gen_close(xgate_object_t *gobj)
{
	XGATE_CLASS_OBJECT_VAR(gobj, XGATE_CLASS_MOD_NAME, gobjc);

	if (gobjc != NULL)
	{
		uint8_t ch_index;

		if (gobjc->task_setup != NULL)
		{
			xtask_delete(gobjc->task_setup);
			gobjc->task_setup = NULL;
		}

		xgate_wave_gen_wave_buffer_free(&gobjc->output_buffer);

		for (ch_index = 0; ch_index < XCOUNTOF(gobjc->ch_buffer); ch_index++)
		{
			xgate_wave_gen_wave_buffer_free(&(gobjc->ch_buffer[ch_index].wave_buffer));
		}
	}

	return (TRUE);
}

static void xgate_wave_gen_poll(xgate_object_t *gobj)
{
	XGATE_CLASS_OBJECT_VAR(gobj, XGATE_CLASS_MOD_NAME, gobjc);
	XGATE_CLASS_CONFIG_VAR(gobj, XGATE_CLASS_MOD_NAME, gconfc);
	uint32_t tick_curr;

	tick_curr = xtask_tick_get();
	if (xtask_tick_erapsed_msec_get(gobjc->setup_tick_prev, tick_curr) >= XGATE_WAVE_GEN_SETUP_IVAL)
	{
		/* 出力 */
		xgate_object_recv(gobj, gobjc->output_buffer.data, gobjc->output_buffer.data_size);

		/* 次のデータ読み込み */
		xgate_wave_gen_output_buffer_setup(gobjc, gconfc);

		gobjc->setup_tick_prev = tick_curr;
	}
}

static void xgate_wave_gen_send(xgate_object_t *gobj, const uint8_t *data, size_t data_size)
{
	return;
}

XGATE_CLASS_DEFINE(XGATE_CLASS_MOD_NAME, XGATE_WAVE_GEN_NAME, XGATE_WAVE_GEN_HELP)

