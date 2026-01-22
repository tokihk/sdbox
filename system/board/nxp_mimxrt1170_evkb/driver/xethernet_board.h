/*
 * xethernet_board.h
 *
 *  Created on: 2024/03/27
 *      Author: Hitoshi Kono
 */

#ifndef XETHERNET_BOARD_H_
#define XETHERNET_BOARD_H_

#include "board.h"

#include "lwip/pbuf.h"

#include "fsl_enet.h"

#include "fsl_phyrtl8201.h"
#include "fsl_phyrtl8211f.h"


#define ENET1G_FRAME_LEN_MAX			(ENET_FRAME_MAX_FRAMELEN + ENET_FRAME_VLAN_TAGLEN)

#define ENET1G_TXBUFF_SIZE				(ENET1G_FRAME_LEN_MAX)
#define ENET1G_RXBUFF_SIZE				(ENET1G_FRAME_LEN_MAX)

#define ENET1G_TXBD_NUM					(24)
#define ENET1G_RXBD_NUM					(24)

#define ENET1G_RXBUFF_NUM				(ENET1G_RXBD_NUM * 2)

#define ENET1G_MAX_BUFFERS_PER_FRAME	((ENET1G_FRAME_LEN_MAX / ENET1G_RXBUFF_SIZE) + ((ENET1G_FRAME_LEN_MAX % ENET1G_RXBUFF_SIZE == 0) ? 0 : 1))


typedef uint8_t xether_enet1g_tx_buff_t[SDK_SIZEALIGN(ENET1G_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT)];
typedef uint8_t xether_enet1g_rx_buff_t[SDK_SIZEALIGN(ENET1G_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT)];


typedef struct xether_rx_pbuf_wrapper
{
	struct pbuf_custom			p;				/*!< Pbuf wrapper. Has to be first. */
	void *						buffer;			/*!< Original buffer wrapped by p. */
	volatile bool_t				buffer_used;	/*!< Wrapped buffer is used by ENET or lwIP. */
} xether_rx_pbuf_wrapper_t;

typedef struct xether_device_status
{
	uint32_t					rx_packet_count;
	uint32_t					tx_packet_count;
} xether_device_status_t;

static struct
{
	ENET_Type *					enet_base;
	enet_handle_t				enet_handle;

	uint32_t					txFlag;

	enet_tx_bd_struct_t *		tx_buff_descrip;
	enet_rx_bd_struct_t *		rx_buff_descrip;

	xether_enet1g_tx_buff_t *	tx_data_buff;
	xether_enet1g_rx_buff_t *	rx_data_buff;

	xether_rx_pbuf_wrapper_t	rxpbuf_list[ENET1G_RXBUFF_NUM];
	uint16_t					rxpbuf_index;

	uint8_t						send_frame_buff[ENET1G_FRAME_LEN_MAX];

	phy_handle_t				phy_handle;
	phy_speed_t					last_speed;
	phy_duplex_t				last_duplex;
	bool_t						last_link_up;
} g_xether_enet1g;


static phy_rtl8201_resource_t		g_enet_phy_resource;
static phy_handle_t					g_enet_phy_handle;

static phy_rtl8211f_resource_t		g_enet1g_phy_resource;


AT_NONCACHEABLE_SECTION_ALIGN(static enet_tx_bd_struct_t g_xether_enet1g_txdesc[ENET1G_TXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(static enet_rx_bd_struct_t g_xether_enet1g_rxdesc[ENET1G_RXBD_NUM], ENET_BUFF_ALIGNMENT);

SDK_ALIGN(static xether_enet1g_tx_buff_t g_xether_enet1g_txbuff[ENET1G_TXBD_NUM], ENET_BUFF_ALIGNMENT);
SDK_ALIGN(static xether_enet1g_rx_buff_t g_xether_enet1g_rxbuff[ENET1G_RXBUFF_NUM], ENET_BUFF_ALIGNMENT);


static const phy_config_t XETHER_ENET1G_PHY_CONFIG =
{
	.phyAddr  = BOARD_ENET1_PHY_ADDRESS,
	.ops      = &phyrtl8211f_ops,
	.resource = &g_enet1g_phy_resource,
	.autoNeg  = true,
};


static void xether_enet1g_mdio_init(void)
{
	CLOCK_EnableClock(s_enetClock[ENET_GetInstance(ENET_1G)]);

	ENET_SetSMI(ENET_1G, CLOCK_GetRootClockFreq(kCLOCK_Root_Bus), false);
}

static status_t xether_enet1g_mdio_write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(ENET_1G, phyAddr, regAddr, data);
}

static status_t xether_enet1g_mdio_read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(ENET_1G, phyAddr, regAddr, pData);
}

static bool_t xether_enet1g_phy_init(void)
{
	status_t status;

	xether_enet1g_mdio_init();

	g_enet1g_phy_resource.write = xether_enet1g_mdio_write;
	g_enet1g_phy_resource.read  = xether_enet1g_mdio_read;

	status = PHY_Init(&g_xether_enet1g.phy_handle, &XETHER_ENET1G_PHY_CONFIG);

	return ((status == kStatus_Success) ? (TRUE) : (FALSE));
}

static void *xether_enet1g_rx_alloc(ENET_Type *base, void *userData, uint8_t ringId)
{
	register uint16_t count;

	for (count = 0; count < ENET1G_RXBUFF_NUM; count++) {
		if (!g_xether_enet1g.rxpbuf_list[count].buffer_used) {
			g_xether_enet1g.rxpbuf_list[count].buffer_used = TRUE;
			return (g_xether_enet1g.rxpbuf_list[count].buffer);
		}
	}

	return (NULL);
}

static void xether_enet1g_rx_free(ENET_Type *base, void *buffer, void *userData, uint8_t ringId)
{
	uint16_t	idx = ((xether_enet1g_rx_buff_t *)buffer) - g_xether_enet1g.rx_data_buff;

	g_xether_enet1g.rxpbuf_list[idx].buffer_used = FALSE;
}

static void xether_enet1g_rx_pbuf_free(struct pbuf *p)
{
	xether_rx_pbuf_wrapper_t *	wrapper = (xether_rx_pbuf_wrapper_t *)p;

	xether_enet1g_rx_free(ENET_1G, wrapper->buffer, NULL, 0);
}

/** Wraps received buffer(s) into a pbuf or a pbuf chain and returns it. */
static struct pbuf *xether_enet1g_rx_frame_to_pbufs(enet_rx_frame_struct_t *rxFrame)
{
	void *buffer;
	uint16_t bufferLength;
	xether_rx_pbuf_wrapper_t *wrapper;
	uint16_t len   = 0U;
	struct pbuf *p = NULL;
	struct pbuf *q = NULL;
	int idx;
	int i;

	for (i = 0; ((i < ENET1G_MAX_BUFFERS_PER_FRAME) && (len < rxFrame->totLen)); i++)
	{
		buffer       = rxFrame->rxBuffArray[i].buffer;
		bufferLength = rxFrame->rxBuffArray[i].length;
		len += bufferLength;

		/* Find pbuf wrapper for the actually read byte buffer */
		idx = ((xether_enet1g_rx_buff_t *)buffer) - g_xether_enet1g.rx_data_buff;
		LWIP_ASSERT("Buffer returned by ENET_GetRxFrame() doesn't match any RX buffer descriptor",
					((idx >= 0) && (idx < ENET1G_RXBUFF_NUM)));
		wrapper = &g_xether_enet1g.rxpbuf_list[idx];
		LWIP_ASSERT("Buffer returned by ENET_GetRxFrame() doesn't match wrapper buffer", wrapper->buffer == buffer);

		/* Wrap the received buffer in pbuf. */
		if (p == NULL)
		{
			p = pbuf_alloced_custom(PBUF_RAW, bufferLength, PBUF_REF, &wrapper->p, buffer, bufferLength);
			LWIP_ASSERT("pbuf_alloced_custom() failed", p);
		}
		else
		{
			q = pbuf_alloced_custom(PBUF_RAW, bufferLength, PBUF_REF, &wrapper->p, buffer, bufferLength);
			LWIP_ASSERT("pbuf_alloced_custom() failed", q);

			pbuf_cat(p, q);
		}
	}

	return (p);
}

static bool_t xether_enet1g_init(const xether_config_t *config)
{
	bool_t					success = FALSE;
	enet_config_t			enet_config;
	enet_buffer_config_t	enet_buff_config;
	uint16_t				i;

	/* Zero clear */
	memset(g_xether_enet1g_txdesc, 0, sizeof(g_xether_enet1g_txdesc));
	memset(g_xether_enet1g_rxdesc, 0, sizeof(g_xether_enet1g_rxdesc));

	/* prepare the buffer configuration. */
	enet_buff_config.txBdNumber      = ENET1G_TXBD_NUM;       /* Transmit buffer descriptor number. */
	enet_buff_config.rxBdNumber      = ENET1G_RXBD_NUM;       /* Receive buffer descriptor number. */
	enet_buff_config.txBuffSizeAlign = sizeof(xether_enet1g_tx_buff_t); /* Aligned transmit data buffer size. */
	enet_buff_config.rxBuffSizeAlign = sizeof(xether_enet1g_rx_buff_t); /* Aligned receive data buffer size. */
	enet_buff_config.txBdStartAddrAlign = &(g_xether_enet1g.tx_buff_descrip[0]); /* Aligned transmit buffer descriptor start address. */
	enet_buff_config.rxBdStartAddrAlign = &(g_xether_enet1g.rx_buff_descrip[0]); /* Aligned receive buffer descriptor start address. */
	enet_buff_config.txBufferAlign = &(g_xether_enet1g.tx_data_buff[0][0]); /* Transmit data buffer start address. */
	enet_buff_config.rxBufferAlign = NULL; /* Receive data buffer start address. NULL when buffers are allocated by callback for RX zero-copy. */
	enet_buff_config.txFrameInfo = NULL; /* Transmit frame information start address. Set only if using zero-copy transmit. */
	enet_buff_config.txMaintainEnable = true; /* Transmit buffer cache maintain. */
	enet_buff_config.rxMaintainEnable = true; /* Receive buffer cache maintain. */

	ENET_GetDefaultConfig(&enet_config);
	enet_config.miiMode = kENET_RgmiiMode;
	enet_config.rxBuffAlloc = xether_enet1g_rx_alloc;
	enet_config.rxBuffFree  = xether_enet1g_rx_free;
	enet_config.userData    = NULL;

	/* Used for detection of change.
	   Initilize to value different than any possible enum value. */
	g_xether_enet1g.last_speed   = (phy_speed_t)0xa5a5;
	g_xether_enet1g.last_duplex  = (phy_duplex_t)0x5a5a;
	g_xether_enet1g.last_link_up = FALSE;

	if (xether_enet1g_phy_init())
	{
		status_t result;

		for (i = 0; i < ENET1G_RXBUFF_NUM; i++)
		{
			g_xether_enet1g.rxpbuf_list[i].p.custom_free_function = xether_enet1g_rx_pbuf_free;
			g_xether_enet1g.rxpbuf_list[i].buffer                 = &(g_xether_enet1g.rx_data_buff[i][0]);
			g_xether_enet1g.rxpbuf_list[i].buffer_used            = FALSE;
		}
		g_xether_enet1g.rxpbuf_index = 0;

		/* Initialize the ENET module. */
		result = ENET_Init(
			ENET_1G,
			&g_xether_enet1g.enet_handle,
			&enet_config,
			&enet_buff_config,
			(uint8_t *)&config->mac_addr.addr[0],
			CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
		);

		if (result == kStatus_Success)
		{
			ENET_ActiveRead(ENET_1G);

			success = TRUE;
		}
	}

	return (success);
}


static bool_t xether_enet1g_open(const xether_config_t *config)
{
	bool_t open_ok = FALSE;

	open_ok = xether_enet1g_init(config);

	return (open_ok);
}

static void xether_enet1g_close(void)
{
}

static struct pbuf *xether_enet1g_recv_packet_get(void)
{
	enet_buffer_struct_t	buffers[ENET1G_MAX_BUFFERS_PER_FRAME];
	enet_rx_frame_struct_t	rxFrame = {.rxBuffArray = &buffers[0] };
	struct pbuf *			p = NULL;
	status_t status;

	/* Read frame. */
//	status = ENET_GetRxFrame_Custom(ENET_1G, &g_xether_enet1g.handle, &rxFrame, 0);
	status = ENET_GetRxFrame(ENET_1G, &g_xether_enet1g.enet_handle, &rxFrame, 0);

	switch (status)
	{
		case kStatus_Success:
			/* Frame read, process it into pbufs. */
			p = xether_enet1g_rx_frame_to_pbufs(&rxFrame);
			break;

		case kStatus_ENET_RxFrameEmpty:
			/* Frame not available. */
			break;

		case kStatus_ENET_RxFrameError:
			/* Error receiving frame */
			break;

		case kStatus_ENET_RxFrameDrop:
			/* Frame received, but it had to be dropped
			 * because new buffer(s) allocation failed in the ENET driver. */
			break;

		default:
			break;
	}

	return (p);
}

static bool_t xether_enet1g_send_packet_set(struct pbuf *p)
{
	err_t		result;
	uint8_t *	pucBuffer = g_xether_enet1g.send_frame_buff;

	/* Initiate transfer. */
	if (p->len == p->tot_len)
	{
		/* No pbuf chain, don't have to copy -> faster. */
		pucBuffer = (uint8_t *)p->payload;
	}
	else
	{
		/* pbuf chain, copy into contiguous ucBuffer. */
		if (p->tot_len > ENET1G_FRAME_LEN_MAX)
		{
			return (ERR_BUF);
		}
		else
		{
			pbuf_copy_partial(p, pucBuffer, p->tot_len, 0);
		}
	}

	/* Send frame. */
	do
	{
		result = ENET_SendFrame(ENET_1G, &g_xether_enet1g.enet_handle, pucBuffer, p->tot_len, 0, false, NULL);
	}
	while (result == kStatus_ENET_TxFrameBusy);

	return (TRUE);
}

static bool_t xether_enet1g_link_status_update(void)
{
	bool	link_status_raw;

	if (PHY_GetLinkStatus(&g_xether_enet1g.phy_handle, &link_status_raw) == kStatus_Success)
	{
		if (g_xether_enet1g.last_link_up != (bool_t)link_status_raw)
		{
			status_t		st;
			phy_speed_t		speed;
			phy_duplex_t	duplex;

			g_xether_enet1g.last_link_up = (bool_t)link_status_raw;

			st = PHY_GetLinkSpeedDuplex(&g_xether_enet1g.phy_handle, &speed, &duplex);
			if (st == kStatus_Success)
			{
				if ((g_xether_enet1g.last_speed != speed) || (g_xether_enet1g.last_duplex != duplex))
				{
					ENET_SetMII(ENET_1G, (enet_mii_speed_t)speed, (enet_mii_duplex_t)duplex);
					g_xether_enet1g.last_speed  = speed;
					g_xether_enet1g.last_duplex = duplex;
				}
			}
		}
	}

	return (g_xether_enet1g.last_link_up);
}

static inline void xether_init_board(void)
{
	g_xether_enet1g.tx_buff_descrip = &(g_xether_enet1g_txdesc[0]);
	g_xether_enet1g.rx_buff_descrip = &(g_xether_enet1g_rxdesc[0]);

	g_xether_enet1g.tx_data_buff    = &(g_xether_enet1g_txbuff[0]);
	g_xether_enet1g.rx_data_buff    = &(g_xether_enet1g_rxbuff[0]);
}

static inline void xether_deinit_board(void)
{
}

XETHERNET_DEVICE_LIST_BEGIN()
  XETHERNET_DEVICE_LIST_ITEM(xether_enet1g),
XETHERNET_DEVICE_LIST_END()

#endif /* XETHER_BOARD_H_ */
