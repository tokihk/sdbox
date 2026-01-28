/*
 * xaudio_board.h
 *
 *  Created on: 2023/01/13
 *      Author: Hitoshi Kono
 */

#ifndef XAUDIO_BOARD_H_
#define XAUDIO_BOARD_H_

#include "driver/xdma_map.h"
#include "driver/xdriver.h"

#include "core/xmalloc.h"

#include "board.h"

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif

#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "fsl_wm8962.h"
#include "fsl_codec_adapter.h"

/* for Debug */
#include "driver/xgpio.h"


#define XAUDIO_SPEAKER_SAMPLING_RATE		(16000)
#define XAUDIO_SPEAKER_BIT_WIDTH			(16)
#define XAUDIO_SPEAKER_BYTE_WIDTH			(XAUDIO_SPEAKER_BIT_WIDTH / 8)
#define XAUDIO_SPEAKER_CH_NUM				(2)

#define XAUDIO_SPEAKER_BUFFER_MSEC			(XCONFIG_AUDIO_BUFFER_MSEC)
#define XAUDIO_SPEAKER_BUFFER_SIZE			(XAUDIO_SPEAKER_SAMPLING_RATE / 1000 * XAUDIO_SPEAKER_BYTE_WIDTH * XAUDIO_SPEAKER_CH_NUM * XAUDIO_SPEAKER_BUFFER_MSEC)

#define XAUDIO_SPEAKER_DMAMUX				DMAMUX0
#define XAUDIO_SPEAKER_DMA					DMA0
#define XAUDIO_SPEAKER_DMA_CH				XBOARD_DMA0_CH0_SAI1_TX

#define XAUDIO_SPEAKER_SAI					SAI1
#define XAUDIO_SPEAKER_SAI_CH				0

#define XAUDIO_SPEAKER_CODEC_VOLUME_MIN		(0)
#define XAUDIO_SPEAKER_CODEC_VOLUME_MAX		(100)


static struct
{
	edma_handle_t				edma_tx_handle;
	codec_handle_t				codec_handle;

	uint8_t *					dma_buffer;
	uint8_t *					dma_buffer_in_ptr;
	size_t						dma_buffer_in_size;

	bool_t						is_active;
	bool_t						is_abort_req;
} g_xaudio_speaker;

/* サイクルバッファ用に２面 */
XSECTION_NONCACHE_DATA(static uint8_t g_xaudio_speaker_dma_buffer[XAUDIO_SPEAKER_BUFFER_SIZE * 2]);

XSECTION_QUICKACCESS_DATA(static edma_tcd_t g_xaudio_speaker_emda_tcd);


static wm8962_config_t g_wm8962_config =
{
	.i2cConfig =
	{
		.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE,
		.codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ
	},
	.route =
	{
		.enableLoopBack            = false,
		.leftInputPGASource        = kWM8962_InputPGASourceInput1,
		.leftInputMixerSource      = kWM8962_InputMixerSourceInputPGA,
		.rightInputPGASource       = kWM8962_InputPGASourceInput3,
		.rightInputMixerSource     = kWM8962_InputMixerSourceInputPGA,
		.leftHeadphoneMixerSource  = kWM8962_OutputMixerDisabled,
		.leftHeadphonePGASource    = kWM8962_OutputPGASourceDAC,
		.rightHeadphoneMixerSource = kWM8962_OutputMixerDisabled,
		.rightHeadphonePGASource   = kWM8962_OutputPGASourceDAC,
	},
	.slaveAddress = WM8962_I2C_ADDR,
	.bus          = kWM8962_BusI2S,
	.format =
	{
		.mclk_HZ    = 24576000U,
		.sampleRate = XAUDIO_SPEAKER_SAMPLING_RATE,
		.bitWidth   = kWM8962_AudioBitWidth16bit
	},
	.masterSlave  = false,
};

static codec_config_t boardCodecConfig =
{
		.codecDevType   = kCODEC_WM8962,
		.codecDevConfig = XAUDIO_SPEAKER_BIT_WIDTH
};

/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM) / (2^POST)
 *                              = 24 * (32 + 768/1000) / 2
 *                              = 393.216 MHZ
 */
static const clock_audio_pll_config_t audioPllConfig =
{
	.loopDivider = 32,   /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
	.postDivider = 1,    /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
	.numerator   = 768,  /* 30 bit numerator of fractional loop divider. */
	.denominator = 1000, /* 30 bit denominator of fractional loop divider */
};


static void xaudio_sai1_mclk_output(bool_t enable)
{
	if (enable)
	{
		BLK_CTRL_NS_AONMIX->SAI1_MCLK_CTRL |= BLK_CTRL_NS_AONMIX_SAI1_MCLK_CTRL_SAI1_MCLK_DIR_MASK;
	}
	else
	{
		BLK_CTRL_NS_AONMIX->SAI1_MCLK_CTRL &= ~BLK_CTRL_NS_AONMIX_SAI1_MCLK_CTRL_SAI1_MCLK_DIR_MASK;
	}
}

static bool_t xaudio_speaker_transfer_start(void)
{
	EDMA_StartTransfer(&g_xaudio_speaker.edma_tx_handle);

#if 1
	/* Enable DMA enable bit */
	SAI_TxEnableDMA(XAUDIO_SPEAKER_SAI, kSAI_FIFORequestDMAEnable, true);

	/* Enable SAI Tx clock */
	SAI_TxEnable(XAUDIO_SPEAKER_SAI, true);

	/* Enable the channel FIFO */
	SAI_TxSetChannelFIFOMask(XAUDIO_SPEAKER_SAI, 1U << XAUDIO_SPEAKER_SAI_CH);
#endif

	return (TRUE);
}

static void xaudio_speaker_transfer_stop_isr(void)
{
	/* DMA stop */
	EDMA_AbortTransfer(&g_xaudio_speaker.edma_tx_handle);

	/* SAI stop */
	SAI_TxEnable(XAUDIO_SPEAKER_SAI, false);

	/* DMAバッファ状態初期化 */
	g_xaudio_speaker.dma_buffer_in_ptr = g_xaudio_speaker.dma_buffer;
	g_xaudio_speaker.dma_buffer_in_size = 0;
}

static void xaudio_speaker_tx_complete_isr(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
	/* for Debug */
	xremcon_signal_out_toggle(3);

	/* DMA転送停止 */
	if (g_xaudio_speaker.is_abort_req)
	{
//		xaudio_speaker_transfer_stop_isr();
//		g_xaudio_speaker.is_active = FALSE;
	}

	/* 不足分を0埋め */
	if (g_xaudio_speaker.dma_buffer_in_size < XAUDIO_SPEAKER_BUFFER_SIZE)
	{
		memcpy(g_xaudio_speaker.dma_buffer_in_ptr + g_xaudio_speaker.dma_buffer_in_size, 0, XAUDIO_SPEAKER_BUFFER_SIZE - g_xaudio_speaker.dma_buffer_in_size);

		/* 不足を検出した場合はDMA転送停止を要求 */
//		g_xaudio_speaker.is_abort_req = TRUE;
	}

	g_xaudio_speaker.dma_buffer_in_size = 0;

	/* 後半ブロックの送信完了 */
	if (transferDone)
	{
		g_xaudio_speaker.dma_buffer_in_ptr = g_xaudio_speaker.dma_buffer + XAUDIO_SPEAKER_BUFFER_SIZE;
	}

	/* 前半ブロックの送信完了 */
	else
	{
		g_xaudio_speaker.dma_buffer_in_ptr = g_xaudio_speaker.dma_buffer;
	}
}

static bool_t xaudio_speaker_sai_init(const xaudio_config_t *config)
{
    sai_transceiver_t sai_config;
	edma_transfer_config_t edma_trans_config;

	EDMA_CreateHandle(&g_xaudio_speaker.edma_tx_handle, XAUDIO_SPEAKER_DMA, XAUDIO_SPEAKER_DMA_CH);
	EDMA_SetCallback(&g_xaudio_speaker.edma_tx_handle, xaudio_speaker_tx_complete_isr, NULL);
	EDMA_ResetChannel(XAUDIO_SPEAKER_DMA, g_xaudio_speaker.edma_tx_handle.channel);

	EDMA_SetChannelMux(XAUDIO_SPEAKER_DMA, XAUDIO_SPEAKER_DMA_CH, kDmaRequestMuxSai1Tx);

	/* SAI Init */
	SAI_Init(XAUDIO_SPEAKER_SAI);

	/* I2S mode configurations */
	SAI_GetClassicI2SConfig(
		&sai_config,
		XAUDIO_SPEAKER_BIT_WIDTH,
		kSAI_Stereo,
		1U << XAUDIO_SPEAKER_SAI_CH
	);
	sai_config.syncMode    = kSAI_ModeAsync;
	sai_config.masterSlave = kSAI_Master;
	SAI_TxSetConfig(XAUDIO_SPEAKER_SAI, &sai_config);

	/* set bit clock divider */
	SAI_TxSetBitClockRate(
		XAUDIO_SPEAKER_SAI,
		CLOCK_GetRootClockFreq(kCLOCK_Root_Sai1),
		XAUDIO_SPEAKER_SAMPLING_RATE,
		XAUDIO_SPEAKER_BIT_WIDTH,
		XAUDIO_SPEAKER_CH_NUM
	);

#if 1

	EDMA_PrepareTransfer(
		&edma_trans_config,
		g_xaudio_speaker.dma_buffer,
		XAUDIO_SPEAKER_BYTE_WIDTH,
		(void *)SAI_TxGetDataRegisterAddress(XAUDIO_SPEAKER_SAI, XAUDIO_SPEAKER_SAI_CH),
		(FSL_FEATURE_SAI_FIFO_COUNTn(XAUDIO_SPEAKER_SAI) - sai_config.fifo.fifoWatermark) * XAUDIO_SPEAKER_BYTE_WIDTH,
		XAUDIO_SPEAKER_BYTE_WIDTH,
		XAUDIO_SPEAKER_BUFFER_SIZE,
		kEDMA_MemoryToPeripheral
	);

	EDMA_TcdSetTransferConfig(
		&g_xaudio_speaker_emda_tcd,
		&edma_trans_config,
		&g_xaudio_speaker_emda_tcd
	);

	EDMA_TcdEnableInterrupts(
		&g_xaudio_speaker_emda_tcd,
		kEDMA_MajorInterruptEnable | kEDMA_HalfInterruptEnable
	);

	EDMA_InstallTCD(
		XAUDIO_SPEAKER_DMA,
		g_xaudio_speaker.edma_tx_handle.channel,
		&g_xaudio_speaker_emda_tcd
	);
#endif

	return (TRUE);
}

static bool_t xaudio_speaker_codec_init(const xaudio_config_t *config)
{
	bool_t			success = TRUE;
	codec_config_t	codec_config = {0};
	uint32_t		volume;

	codec_config.codecDevType   = kCODEC_WM8962;
	codec_config.codecDevConfig = &g_wm8962_config;

	volume = ((uint32_t)config->volume * (XAUDIO_SPEAKER_CODEC_VOLUME_MAX - XAUDIO_SPEAKER_CODEC_VOLUME_MIN) / 255) + XAUDIO_SPEAKER_CODEC_VOLUME_MIN;

	/* Use default setting to init codec */
	if (CODEC_Init(&g_xaudio_speaker.codec_handle, &codec_config) != kStatus_Success)
	{
		success = FALSE;
	}

	else if (CODEC_SetVolume(
			  &g_xaudio_speaker.codec_handle,
			  kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
			  volume
		   ) != kStatus_Success)
	{
		success = FALSE;
	}

	xdriver_wait_us(10000);

	return (success);
}


static bool_t xaudio_speaker_hw_init(const xaudio_config_t *config)
{
	bool_t success = TRUE;

	/* SAI setting */
	if (!xaudio_speaker_sai_init(config))
	{
		success = FALSE;
	}

	/* Codec setting */
	else if (!xaudio_speaker_codec_init(config))
	{
		success = FALSE;
	}

	/* Transfer start */
	else if (!xaudio_speaker_transfer_start())
	{
		success = FALSE;
	}

	return (success);
}

static bool_t xaudio_speaker_open(const xaudio_config_t *config, xaudio_open_device_info_t *odi)
{
	bool_t success = FALSE;

#if 0
	/* デバイス情報取得 */
	g_xaudio_speaker.bits_per_sample = ((config->bits_per_sample + 7) >> 3) << 3;
	g_xaudio_speaker.bits_per_sample = XMAX(g_xaudio_speaker.bits_per_sample, 16);
	g_xaudio_speaker.bits_per_sample = XMIN(g_xaudio_speaker.bits_per_sample, 32);

	switch (g_xaudio_speaker.bits_per_sample) {
		case 16:	odi->pcm_out_format = XAUDIO_DATA_FORMAT_U16BE;		break;
		case 24:	odi->pcm_out_format = XAUDIO_DATA_FORMAT_U24BE;		break;
		case 32:	odi->pcm_out_format = XAUDIO_DATA_FORMAT_U32BE;		break;
		default:	odi->pcm_out_format = XAUDIO_DATA_FORMAT_U16BE;		break;
	}
#else
	odi->pcm_out_format = XPCM_DATA_FORMAT_U16BE;
	odi->pcm_out_ch = XAUDIO_SPEAKER_CH_NUM;
#endif

	/* DMAバッファ初期化 */
	g_xaudio_speaker.dma_buffer = g_xaudio_speaker_dma_buffer;
	g_xaudio_speaker.dma_buffer_in_ptr = g_xaudio_speaker.dma_buffer;
	g_xaudio_speaker.dma_buffer_in_size = 0;

	/* DMAバッファクリア */
	memset(g_xaudio_speaker.dma_buffer, 0, sizeof(g_xaudio_speaker_dma_buffer));

	/* ハードウェア初期化 */
	if (xaudio_speaker_hw_init(config)) {
		/* 成功 */
		success = TRUE;
	}

	return (success);
}

static void xaudio_speaker_close(void)
{
	xaudio_speaker_transfer_stop_isr();
}

static void xaudio_speaker_mute_set(bool_t mute_off)
{
	CODEC_SetMute(
		&g_xaudio_speaker.codec_handle,
		kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
		(mute_off) ? (false) : (true)
	);
}

static size_t xaudio_speaker_pcm_data_add(const uint8_t *data, size_t data_size)
{
	size_t add_size = 0;
	bool_t active_req = FALSE;

	DisableIRQ(DMA0_DMA16_IRQn);
	{
		if (!g_xaudio_speaker.is_abort_req)
		{
			/* DMAバッファへデータ設定 */
			while (   (add_size < data_size)
				   && (g_xaudio_speaker.dma_buffer_in_size < XAUDIO_SPEAKER_BUFFER_SIZE)
			){
				g_xaudio_speaker.dma_buffer_in_ptr[g_xaudio_speaker.dma_buffer_in_size++] = data[add_size++];
			}

			/* DMA転送停止中かつバッファがフルになったときにDMA転送開始 */
			if (   (!g_xaudio_speaker.is_active)
				&& (g_xaudio_speaker.dma_buffer_in_size >= XAUDIO_SPEAKER_BUFFER_SIZE)
			){
				g_xaudio_speaker.is_active = TRUE;
//				xaudio_speaker_transfer_start();
			}
		}
	}
	EnableIRQ(DMA0_DMA16_IRQn);

	return (add_size);
}

static inline void xaudio_init_board(void)
{
	/* Init Audio Pll. */
	CLOCK_InitAudioPll(&audioPllConfig);

	/*Clock setting for SAI1*/
	CLOCK_SetRootClockMux(kCLOCK_Root_Sai1, kCLOCK_SAI1_ClockRoot_MuxAudioPllOut);

	/* Init LPI2C PLL for WM8962 */
	CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, 32);

	/*Clock setting for LPI2C*/
	CLOCK_SetRootClockMux(kCLOCK_Root_Lpi2c0102, kCLOCK_LPI2C0102_ClockRoot_MuxOscRc24M);
	CLOCK_SetRootClockDiv(kCLOCK_Root_Lpi2c0102, 1);
}

static inline void xaudio_deinit_board(void)
{
}

XAUDIO_DEVICE_LIST_BEGIN()
  XAUDIO_DEVICE_LIST_ITEM(xaudio_speaker),
XAUDIO_DEVICE_LIST_END()

#endif /* XAUDIO_BOARD_H_ */
