/*
 * xuart_board.h
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */

#ifndef XUART_BOARD_H_
#define XUART_BOARD_H_

#include "driver/xuart.h"
#include "driver/xdma_map.h"

#include "board.h"

#include "fsl_lpuart.h"
#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"


#define XUART_LPUART1_TX_CH_NUM				(2)
#define XUART_LPUART1_TX_BUFFER_SIZE		(256)
#define XUART_LPUART1_RX_BUFFER_SIZE		(128)
#define XUART_LPUART1_TX_CH_BUFFER_SIZE		(XUART_LPUART1_TX_BUFFER_SIZE / XUART_LPUART1_TX_CH_NUM)

#define XUART_LPUART1_DMAMUX				(DMAMUX0)
#define XUART_LPUART1_DMA					(DMA0)
#define XUART_LPUART1_DMA_CH_TX				(XBOARD_DMA0_CH0_LPUART1_TX)
#define XUART_LPUART1_DMA_CH_RX				(XBOARD_DMA0_CH0_LPUART1_RX)


static struct
{
	lpuart_config_t				lpuart_config;
	lpuart_transfer_t			xfer;
	lpuart_transfer_t			sendXfer;
	lpuart_transfer_t			receiveXfer;

	lpuart_edma_handle_t		lpuart_handle;
	edma_handle_t				edma_tx_handle;
	edma_handle_t				edma_rx_handle;

	lpuart_transfer_t			xfer_tx[XUART_LPUART1_TX_CH_NUM];
	uint8_t						xfer_tx_ch;
	lpuart_transfer_t *			xfer_tx_cur;
	bool_t						xfer_tx_setup_busy;

	uint8_t *					dma_rx_buffer_ptr;
} g_xuart_lpuart1;


AT_NONCACHEABLE_SECTION_INIT(static uint8_t g_xuart_lpuart1_tx_buffer[XUART_LPUART1_TX_BUFFER_SIZE]);
AT_NONCACHEABLE_SECTION_INIT(static uint8_t g_xuart_lpuart1_rx_buffer[XUART_LPUART1_RX_BUFFER_SIZE]);

AT_QUICKACCESS_SECTION_DATA_ALIGN(static edma_tcd_t g_xuart_lpuart1_tcd[2], sizeof(edma_tcd_t));


static inline void xuart_lpuart1_tx_start_isr(void)
{
	/* 送信データ入力ではない and 現在の送信バッファにデータがあるときのみ送信開始 */
	if (   (g_xuart_lpuart1.xfer_tx_cur->dataSize > 0)
		&& (!g_xuart_lpuart1.xfer_tx_setup_busy)
	) {
		if (LPUART_SendEDMA(LPUART1, &g_xuart_lpuart1.lpuart_handle, g_xuart_lpuart1.xfer_tx_cur) == kStatus_Success) {
			/* 次の送信バッファCHを取得 */
			if (g_xuart_lpuart1.xfer_tx_ch < (XCOUNTOF(g_xuart_lpuart1.xfer_tx) - 1)) {
				g_xuart_lpuart1.xfer_tx_ch++;
			} else {
				g_xuart_lpuart1.xfer_tx_ch = 0;
			}

			g_xuart_lpuart1.xfer_tx_cur = &g_xuart_lpuart1.xfer_tx[g_xuart_lpuart1.xfer_tx_ch];

			/* 次の送信バッファを初期化 */
			g_xuart_lpuart1.xfer_tx_cur->dataSize = 0;
		}
	}
}

static inline void xuart_lpuart1_tx_start(void)
{
//	XDI();
	DisableIRQ(LPUART1_IRQn);
	{
		xuart_lpuart1_tx_start_isr();
	}
	EnableIRQ(LPUART1_IRQn);
//	XEI();
}


static void xuart_lpuart1_callback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData)
{
	if (status == kStatus_LPUART_TxIdle)
	{
		xuart_lpuart1_tx_start_isr();
	}
}

bool_t xuart_lpuart1_open(const xuart_config_t *config)
{
	/* --- UART Initialize --- */
	{
		lpuart_config_t lpuart_conf;

		LPUART_GetDefaultConfig(&lpuart_conf);

		/* baud rate */
		lpuart_conf.baudRate_Bps = config->baudrate;

		/* parity */
		switch (config->parity) {
			case XUART_PARITY_NONE:		lpuart_conf.parityMode = kLPUART_ParityDisabled;	break;
			case XUART_PARITY_EVEN:		lpuart_conf.parityMode = kLPUART_ParityEven;		break;
			case XUART_PARITY_ODD:		lpuart_conf.parityMode = kLPUART_ParityOdd;			break;
			default:					lpuart_conf.parityMode = kLPUART_ParityDisabled;	break;
		}

		/* data bits */
		lpuart_conf.dataBitsCount = (config->data_bits == 7) ? (kLPUART_SevenDataBits) : (kLPUART_EightDataBits);

		/* stop bits */
		lpuart_conf.stopBitCount = (config->stop_bits == XUART_STOPBIT_TWO) ? (kLPUART_TwoStopBit) : (kLPUART_OneStopBit);

		lpuart_conf.enableTx     = true;
		lpuart_conf.enableRx     = true;

		LPUART_Init(LPUART1, &lpuart_conf, CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart1));
	}

	/* DMAMUX Initialize */
	{
		/* Set channel for LPUART */
		DMAMUX_SetSource(XUART_LPUART1_DMAMUX, XUART_LPUART1_DMA_CH_TX, 8);
		DMAMUX_SetSource(XUART_LPUART1_DMAMUX, XUART_LPUART1_DMA_CH_RX, 9);
		DMAMUX_EnableChannel(DMAMUX0, XUART_LPUART1_DMA_CH_TX);
		DMAMUX_EnableChannel(DMAMUX0, XUART_LPUART1_DMA_CH_RX);
	}

	/* DMA Initialize */
	{
		edma_config_t edma_conf;

		EDMA_GetDefaultConfig(&edma_conf);

		EDMA_Init(XUART_LPUART1_DMA, &edma_conf);
		EDMA_CreateHandle(&g_xuart_lpuart1.edma_tx_handle, XUART_LPUART1_DMA, XUART_LPUART1_DMA_CH_TX);
		EDMA_CreateHandle(&g_xuart_lpuart1.edma_rx_handle, XUART_LPUART1_DMA, XUART_LPUART1_DMA_CH_RX);
	}

	/* DMA TCD Setting */
	{
	    edma_transfer_config_t trans_config;

		EDMA_PrepareTransfer(
			&trans_config,
			(void *)(uint32_t *)LPUART_GetDataRegisterAddress(LPUART1),
			sizeof(uint8_t),
			g_xuart_lpuart1_rx_buffer,
			sizeof(uint8_t),
			sizeof(uint8_t),
			sizeof(g_xuart_lpuart1_rx_buffer),
			kEDMA_PeripheralToMemory
		);
		EDMA_TcdSetTransferConfig(
			&g_xuart_lpuart1_tcd[0],
			&trans_config,
			&g_xuart_lpuart1_tcd[0]
		);

		EDMA_InstallTCD(XUART_LPUART1_DMA, XUART_LPUART1_DMA_CH_RX, &g_xuart_lpuart1_tcd[0]);
	}

	/* Create LPUART DMA handle. */
	LPUART_TransferCreateHandleEDMA(
		LPUART1,
		&g_xuart_lpuart1.lpuart_handle,
		xuart_lpuart1_callback,
		NULL,
		&g_xuart_lpuart1.edma_tx_handle,
		&g_xuart_lpuart1.edma_rx_handle
	);

	/* 送信バッファ初期化 */
	{
		uint8_t ch_index;

		for (ch_index = 0; ch_index < XCOUNTOF(g_xuart_lpuart1.xfer_tx); ch_index++) {
			g_xuart_lpuart1.xfer_tx[ch_index].data =
				&g_xuart_lpuart1_tx_buffer[XCOUNTOF(g_xuart_lpuart1_tx_buffer) / XCOUNTOF(g_xuart_lpuart1.xfer_tx) * ch_index];
			g_xuart_lpuart1.xfer_tx[ch_index].dataSize = 0;
		}

		g_xuart_lpuart1.xfer_tx_ch = 0;
		g_xuart_lpuart1.xfer_tx_cur = &g_xuart_lpuart1.xfer_tx[g_xuart_lpuart1.xfer_tx_ch];
	}

	/* 受信バッファ初期化 */
	{
		g_xuart_lpuart1.dma_rx_buffer_ptr = g_xuart_lpuart1_rx_buffer;
	}

	/* 受信開始 */
	EDMA_StartTransfer(&g_xuart_lpuart1.edma_rx_handle);
	LPUART_EnableRxDMA(LPUART1, true);

	return (TRUE);
}

void xuart_lpuart1_close(void)
{
	EDMA_AbortTransfer(&g_xuart_lpuart1.edma_rx_handle);
	LPUART_EnableRxDMA(LPUART1, false);
}

size_t xuart_lpuart1_read(uint8_t *buffer, size_t buffer_size)
{
	register uint8_t *buffer_ptr = buffer;
	register uint8_t *buffer_ptr_end = buffer + buffer_size;
	register uint8_t *dma_buffer_ptr = g_xuart_lpuart1.dma_rx_buffer_ptr;
	register uint8_t *dma_buffer_ptr_end = (uint8_t *)(XUART_LPUART1_DMA->TCD[g_xuart_lpuart1.edma_rx_handle.channel].DADDR);

	while (   (buffer_ptr < buffer_ptr_end)
		   && (dma_buffer_ptr != dma_buffer_ptr_end)
	) {
		*buffer_ptr++ = *dma_buffer_ptr++;

		if (dma_buffer_ptr >= &g_xuart_lpuart1_rx_buffer[XCOUNTOF(g_xuart_lpuart1_rx_buffer)]) {
			dma_buffer_ptr = g_xuart_lpuart1_rx_buffer;
		}
	}

	g_xuart_lpuart1.dma_rx_buffer_ptr = dma_buffer_ptr;

	return ((size_t)(buffer_ptr - buffer));
}

size_t xuart_lpuart1_write(const uint8_t *data, size_t data_size)
{
	uint16_t write_size = 0;

	g_xuart_lpuart1.xfer_tx_setup_busy = TRUE;
	{
		lpuart_transfer_t *xfer_tx = g_xuart_lpuart1.xfer_tx_cur;

		if (data_size > (XUART_LPUART1_TX_CH_BUFFER_SIZE - xfer_tx->dataSize))
		{
			data_size = XUART_LPUART1_TX_CH_BUFFER_SIZE - xfer_tx->dataSize;
		}

		while (write_size < data_size)
		{
			xfer_tx->data[xfer_tx->dataSize] = data[write_size];
			xfer_tx->dataSize++;
			write_size++;
		}
	}
	g_xuart_lpuart1.xfer_tx_setup_busy = FALSE;

	xuart_lpuart1_tx_start();

	return (write_size);
}

static inline void xuart_init_board(void)
{
}

static inline void xuart_deinit_board(void)
{
}

XUART_DEVICE_LIST_BEGIN()
//  XUART_DEVICE_LIST_ITEM(xuart_lpuart1),
XUART_DEVICE_LIST_END()

#endif /* XUART_BOARD_H_ */
