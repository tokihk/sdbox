/*
 * xaudio_board.h
 *
 *  Created on: 2023/01/13
 *      Author: Hitoshi Kono
 */

#ifndef XAUDIO_BOARD_H_
#define XAUDIO_BOARD_H_

#include "driver/xdriver.h"

#include "config/xconfig_const.h"

#include "core/xmalloc.h"

#include "general/xwavegen.h"

#include <alsa/asoundlib.h>


#define XAUDIO_posix_PCM_BLOCK_NUM			(2)
#define XAUDIO_posix_PCM_BLOCK_SIZE_MSEC	(50)


typedef struct xaudio_posix_pcm_block
{
//	WAVEHDR						whdr;
	uint8_t *					pcm_buffer;
	size_t						pcm_data_size;
	bool_t						is_busy;
} xaudio_posix_pcm_block_t;

static struct
{
//	HWAVEOUT					hwout;
//	WAVEFORMATEX				wfmt;

	uint8_t *					pcm_buffer;
	size_t						pcm_buffer_size;
	size_t						pcm_block_size;

	xaudio_posix_pcm_block_t	pcm_block[XAUDIO_posix_PCM_BLOCK_NUM];
	uint8_t						pcm_block_in;

	bool_t						waveout_active;
} g_xaudio_posix;


#if 0
static void CALLBACK xaudio_posix_waveout_proc
(
   HWAVEOUT  hwo,
   UINT      uMsg,
   DWORD_PTR dwInstance,
   DWORD_PTR dwParam1,
   DWORD_PTR dwParam2
) {
	switch (uMsg)
	{
		case WOM_DONE:
		{
			if ((((LPWAVEHDR)dwParam1)->dwFlags & WHDR_DONE) != 0)
			{
				xaudio_posix_pcm_block_t *block = (xaudio_posix_pcm_block_t *)(((LPWAVEHDR)dwParam1)->dwUser);

				/* 出力したブロックを空に設定 */
				block->pcm_data_size = 0;
				block->is_busy = FALSE;
			}
		}
			break;

		case WOM_OPEN:
			break;

		case WOM_CLOSE:
			break;

		default:
			break;
	}
}
#endif

static void	xaudio_posix_close(void)
{
#if 0
	if (g_xaudio_posix.hwout != NULL)
	{
		xaudio_posix_pcm_block_t *	block;
		uint8_t						block_index;

		/* 再生を停止 */
		waveOutReset(g_xaudio_posix.hwout);

		/* 出力バッファ解放準備 */
		for (block_index = 0; block_index < XCOUNTOF(g_xaudio_posix.pcm_block); block_index++)
		{
			block = &g_xaudio_posix.pcm_block[block_index];

			waveOutUnprepareHeader(g_xaudio_posix.hwout, &block->whdr, sizeof(block->whdr));
		}

		/* デバイスクローズ */
		waveOutClose(g_xaudio_posix.hwout);
		g_xaudio_posix.hwout = NULL;
	}
#endif

	/* 出力バッファ解放 */
	if (g_xaudio_posix.pcm_buffer != NULL)
	{
		xfree(g_xaudio_posix.pcm_buffer);
		g_xaudio_posix.pcm_buffer = NULL;
		g_xaudio_posix.pcm_buffer_size = 0;
	}
}

static bool_t xaudio_posix_open(const xaudio_config_t *config, xaudio_open_device_info_t *odi)
{
	bool_t						success = FALSE;

#if 0
	WAVEFORMATEX				wfmt = {0};
	const xpcm_format_info_t *	format_info_req;
	xpcm_format_info_t			format_info_dev;
	MMRESULT					result;

	/* Windowsオーディオデバイス設定 */
	format_info_req = xpcm_format_info_get(config->pcm_format);
	if (format_info_req == NULL)
	{
		/* Error: サポート外フォーマット */
		return (FALSE);
	}

	wfmt.wFormatTag = WAVE_FORMAT_PCM;
	wfmt.nChannels = config->channel;
	wfmt.nSamplesPerSec = config->sampling_rate;
	wfmt.wBitsPerSample = (WORD)(format_info_req->size) << 3;

	wfmt.nBlockAlign = wfmt.nChannels * (wfmt.wBitsPerSample >> 3);
	wfmt.nAvgBytesPerSec = wfmt.nBlockAlign * wfmt.nSamplesPerSec;

	/* デバイスオープン */
	result = waveOutOpen(&g_xaudio_posix.hwout, WAVE_MAPPER, &wfmt, (DWORD_PTR)xaudio_posix_waveout_proc, 0, CALLBACK_FUNCTION);

	if (result == MMSYSERR_NOERROR)
	{
		/* 出力バッファサイズ算出 */
		g_xaudio_posix.pcm_block_size = XPCM_DATA_SIZE_GET(wfmt.nSamplesPerSec,wfmt.wBitsPerSample >> 3,wfmt.nChannels,XAUDIO_posix_PCM_BLOCK_SIZE_MSEC);
		g_xaudio_posix.pcm_buffer_size = g_xaudio_posix.pcm_block_size * XAUDIO_posix_PCM_BLOCK_NUM;

		/* 出力バッファ確保 */
		g_xaudio_posix.pcm_buffer = xmalloc(g_xaudio_posix.pcm_buffer_size);
		if (g_xaudio_posix.pcm_buffer != NULL)
		{
			xaudio_posix_pcm_block_t *	block;
			uint8_t						block_index;

			for (block_index = 0; block_index < XCOUNTOF(g_xaudio_posix.pcm_block); block_index++)
			{
				block = &g_xaudio_posix.pcm_block[block_index];

				block->pcm_buffer = &g_xaudio_posix.pcm_buffer[g_xaudio_posix.pcm_block_size * block_index];
				block->pcm_data_size = 0;
				block->is_busy = FALSE;

				block->whdr.lpData = (LPSTR)block->pcm_buffer;
				block->whdr.dwBufferLength = g_xaudio_posix.pcm_block_size;
				block->whdr.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
				block->whdr.dwLoops = 1;
				block->whdr.dwUser = (DWORD_PTR)block;

				result = waveOutPrepareHeader(g_xaudio_posix.hwout, &block->whdr, sizeof(block->whdr));
				if (result != MMSYSERR_NOERROR) {
					break;
				}
			}

			if (result == MMSYSERR_NOERROR)
			{
				/* 実際にAPIに設定した値のフォーマットを取得 */
				format_info_dev.is_little = TRUE;
				format_info_dev.is_signed = FALSE;
				format_info_dev.size = (uint8_t)(wfmt.wBitsPerSample >> 3);

				odi->pcm_out_sampling_rate = wfmt.nSamplesPerSec;
				odi->pcm_out_format = xpcm_format_type_get(&format_info_dev);
				odi->pcm_out_ch = (uint8_t)wfmt.nChannels;

				g_xaudio_posix.pcm_block_in = 0;

				success = TRUE;
			}
		}
	}

	if (!success)
	{
		xaudio_posix_close();
	}
#endif

	return (success);
}

static void	xaudio_posix_mute_set(bool_t mute_off)
{

}

static size_t xaudio_posix_pcm_data_add(const uint8_t *data, size_t data_size)
{
	size_t						add_size = 0;
	xaudio_posix_pcm_block_t *	block = &g_xaudio_posix.pcm_block[g_xaudio_posix.pcm_block_in];

	/* PCMブロックが出力中ではないときのみ */
	if (!block->is_busy)
	{
		/* コピーサイズ算出 */
		add_size = g_xaudio_posix.pcm_block_size - block->pcm_data_size;
		if (add_size >= data_size)
		{
			add_size = data_size;
		}

		/* 出力バッファへデータコピー */
		if (add_size > 0)
		{
			memcpy(&block->pcm_buffer[block->pcm_data_size], data, add_size);
			block->pcm_data_size += add_size;
		}

		/* 出力バッファがフルになった */
		if (block->pcm_data_size >= g_xaudio_posix.pcm_block_size)
		{
			block->is_busy = TRUE;

			/* 出力キューへ設定 */
//			waveOutWrite(g_xaudio_posix.hwout, &block->whdr, sizeof(block->whdr));

			/* 次のPCMブロックを設定 */
			g_xaudio_posix.pcm_block_in = (g_xaudio_posix.pcm_block_in + 1) % XCOUNTOF(g_xaudio_posix.pcm_block);
		}
	}

	return (add_size);
}

static inline void xaudio_init_board(void)
{
}

static inline void xaudio_deinit_board(void)
{
}

XAUDIO_DEVICE_LIST_BEGIN()
  XAUDIO_DEVICE_LIST_ITEM(xaudio_posix),
XAUDIO_DEVICE_LIST_END()

#endif /* XAUDIO_BOARD_H_ */
