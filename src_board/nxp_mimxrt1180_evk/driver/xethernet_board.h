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

#include "netc/fsl_netc.h"
#include "netc/fsl_netc_mdio.h"
#include "netc/fsl_netc_switch.h"
#include "netc/fsl_netc_endpoint.h"

#include "fsl_msgintr.h"

#include "fsl_phyrtl8201.h"
#include "fsl_phyrtl8211f.h"


#define NETC_PHY_PAGE_SELECT_REG		0x1FU	/*!< The PHY page select register. */

#define NETC_EP0_MSGINTR				MSGINTR1

#define NETC0_TX_INTR_MSG_DATA			1U
#define NETC0_RX_INTR_MSG_DATA			2U
#define NETC1_TX_INTR_MSG_DATA			3U
#define NETC1_RX_INTR_MSG_DATA			4U

#define NETC_EP_TX_MSIX_ENTRY_IDX		0U
#define NETC_EP_RX_MSIX_ENTRY_IDX		1U

#define NETC_EP_BUFF_SIZE_ALIGN			64U
#define NETC_EP_BD_ALIGN				128U

#define NETC_EP_RXBUFF_SIZE				1522U
#define NETC_EP_RXBUFF_SIZE_ALIGN		SDK_SIZEALIGN(NETC_EP_RXBUFF_SIZE, NETC_EP_BUFF_SIZE_ALIGN)
#define NETC_EP_TXBUFF_SIZE				1522U
#define NETC_EP_TXBUFF_SIZE_ALIGN		SDK_SIZEALIGN(NETC_EP_TXBUFF_SIZE, NETC_EP_BUFF_SIZE_ALIGN)

#define NETC_EP0_RXRING_NUM				3U
#define NETC_EP0_RXBD_NUM				8U

#define NETC_EP0_TXRING_NUM				3U
#define NETC_EP0_TXFRAME_NUM			20U
#define NETC_EP0_TXBD_NUM				8U


typedef enum xether_netc_ep_id
{
	XETHER_NETC_EP0,				/* ENETC0 */
	XETHER_NETC_EP1,				/* ENETC1 */

	XETHER_NETC_EP_MAX
} xether_netc_ep_id_t;

typedef enum xether_netc_port_id
{
	XETHER_NETC_PORT_ETH0,
	XETHER_NETC_PORT_ETH1,
	XETHER_NETC_PORT_ETH2,
	XETHER_NETC_PORT_ETH3,
	XETHER_NETC_PORT_ETH4,

	XETHER_NETC_PORT_MAX
} xether_netc_port_id_t;

typedef enum xether_netc_msix_entry_id
{
	XETHER_NETC_MSIX_ENTRYID_RX,
	XETHER_NETC_MSIX_ENTRYID_TX,

	XETHER_NETC_MSIX_ENTRYID_MAX,
} xether_netc_msix_entry_id_t;

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

typedef struct xether_netc_config
{
	netc_hw_si_idx_t			hw_si_id;
} xether_netc_config_t;

typedef struct xether_ep_status
{
	phy_speed_t					last_speed;
	phy_duplex_t				last_duplex;
	bool_t						last_link_up;
} xether_ep_status_t;

typedef struct xether_netc_port_handle
{
	phy_handle_t				phy_handle;
	phy_rtl8211f_resource_t		phy_resource;

	netc_hw_si_idx_t			hw_si_id;

	xether_ep_status_t			status;
} xether_netc_port_handle_t;

static struct
{
	ep_handle_t					ep_handle[XETHER_NETC_EP_MAX];

	netc_mdio_handle_t			emdio_handle;

	phy_rtl8211f_resource_t		phy_resource[XETHER_NETC_PORT_MAX];
	phy_handle_t				phy_handle[XETHER_NETC_PORT_MAX];

	uint32_t					txFlag;

	phy_speed_t					last_speed;
	phy_duplex_t				last_duplex;
	bool_t						last_link_up;
} g_xether_board;


typedef uint8_t												netc_ep_rx_buffer_t[NETC_EP_RXBUFF_SIZE_ALIGN];
typedef uint8_t												netc_ep_tx_buffer_t[NETC_EP_TXBUFF_SIZE_ALIGN];

AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t			g_xether_netc0_rxbuff_descriptor[NETC_EP_RXRING_NUM][NETC_EP_RXBD_NUM], NETC_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_ep_rx_buffer_t	g_xether_netc0_rxdata_buff[NETC_EP_RXRING_NUM][NETC_EP_RXBD_NUM], NETC_EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t				g_xether_netc0_rxframe[NETC_EP_RXBUFF_SIZE_ALIGN], NETC_EP_BUFF_SIZE_ALIGN);

AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t			g_xether_netc0_txbuff_descriptor[NETC_EP_TXRING_NUM][NETC_EP_TXBD_NUM], NETC_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_ep_tx_buffer_t	g_xether_netc0_txframe[NETC_EP_TXBUFF_SIZE], NETC_EP_BUFF_SIZE_ALIGN);

AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t			g_xether_netc0_mgmt_txBuff_descriptor[NETC_EP_TXBD_NUM], NETC_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t			g_xether_netc0_cmd_buff_descriptor[NETC_EP_TXBD_NUM], NETC_EP_BD_ALIGN);

static uint64_t												g_xether_netc0_rxbuff_addr_array[NETC_EP_RXRING_NUM][NETC_EP_RXBD_NUM];
static netc_tx_frame_info_t									g_xether_netc0_mgmt_txdirty[NETC_EP_TXBD_NUM];
static netc_tx_frame_info_t									g_xether_netc0_mgmt_txframe_info;

static netc_tx_frame_info_t									g_xether_netc0_tx_dirty[NETC_EP_TXRING_NUM][NETC_EP_TXBD_NUM];


static inline void xether_netc_ep0_phy_reset_pin_set(bool_t reset)
{
	RGPIO_PinWrite(RGPIO4, 13, (reset) ? (0) : (1));
}

static inline void xether_netc_ep1_phy_reset_pin_set(bool_t reset)
{
	RGPIO_PinWrite(RGPIO4, 25, (reset) ? (0) : (1));
}

static inline void xether_netc_ep2_phy_reset_pin_set(bool_t reset)
{
	RGPIO_PinWrite(RGPIO6, 13, (reset) ? (0) : (1));
}

static inline void xether_netc_ep3_phy_reset_pin_set(bool_t reset)
{
	RGPIO_PinWrite(RGPIO4, 28, (reset) ? (0) : (1));
}

static inline void xether_netc_ep4_phy_reset_pin_set(bool_t reset)
{
	RGPIO_PinWrite(RGPIO6, 15, (reset) ? (0) : (1));
}

status_t xether_netc_mdio_init(void)
{
	netc_mdio_config_t mdio_config =
	{
			.mdio.type			= kNETC_EMdio,
			.isPreambleDisable	= false,
			.isNegativeDriven	= false,
			.srcClockHz			= CLOCK_GetRootClockFreq(kCLOCK_Root_Netc),
	};

    return (NETC_MDIOInit(&g_xether_board.emdio_handle, &mdio_config));
}

static status_t xether_netc_phy_mdio_write(uint8_t phy_addr, uint8_t reg_addr, uint16_t data)
{
	return (NETC_MDIOWrite(&g_xether_board.emdio_handle, phy_addr, reg_addr, data));
}

static status_t xether_netc_phy_mdio_read(uint8_t phy_addr, uint8_t reg_addr, uint16_t *buffer)
{
	return (NETC_MDIORead(&g_xether_board.emdio_handle, phy_addr, reg_addr, buffer));
}

static status_t xether_netc_phy_setup_rtl8201(phy_handle_t *handle)
{
	status_t result;
	uint16_t data;

	result = PHY_Write(handle, NETC_PHY_PAGE_SELECT_REG, 7);
	if (result != kStatus_Success)
	{
		return result;
	}

	result = PHY_Read(handle, 16, &data);
	if (result != kStatus_Success)
	{
		return result;
	}

	/* CRS/DV pin is RXDV signal. */
	data |= (1U << 2);
	result = PHY_Write(handle, 16, data);
	if (result != kStatus_Success)
	{
		return result;
	}

	result = PHY_Write(handle, NETC_PHY_PAGE_SELECT_REG, 0);

	return (result);
}

static status_t xether_netc_phy_init(uint32_t port, phy_config_t *phy_config)
{
    if (port >= XCOUNTOF(g_xether_board.phy_resource))
    {
    	return (kStatus_OutOfRange);
    }

	g_xether_board.phy_resource[port].write = xether_netc_phy_mdio_write;
	g_xether_board.phy_resource[port].read  = xether_netc_phy_mdio_read;

	return (PHY_Init(&g_xether_board.phy_handle[port], phy_config));
}

status_t xether_netc_phy_link_status_get(uint32_t port, bool *link)
{
	return (PHY_GetLinkStatus(&g_xether_board.phy_handle[port], link));
}


static status_t xether_netc_port_init(void)
{
    status_t result = kStatus_Success;

    phy_config_t phy8211Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed1000M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8211f_ops,
    };
    phy_config_t phy8201Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8201_ops,
    };

    /* Reset all PHYs even some are not used in case unstable status has effect on other PHYs. */
    /* Reset PHY8201 for ETH4(EP), ETH0(Switch port0). Power on 150ms, reset 10ms, wait 150ms. */
    /* Reset PHY8211 for ETH1(Switch port1), ETH2(Switch port2), ETH3(Switch port3). Reset 10ms, wait 30ms. */
    xether_netc_ep0_phy_reset_pin_set(TRUE);
    xether_netc_ep1_phy_reset_pin_set(TRUE);
    xether_netc_ep2_phy_reset_pin_set(TRUE);
    xether_netc_ep3_phy_reset_pin_set(TRUE);
    xether_netc_ep4_phy_reset_pin_set(TRUE);

    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));

    xether_netc_ep0_phy_reset_pin_set(FALSE);
    xether_netc_ep1_phy_reset_pin_set(FALSE);
    xether_netc_ep2_phy_reset_pin_set(FALSE);
    xether_netc_ep3_phy_reset_pin_set(FALSE);
    xether_netc_ep4_phy_reset_pin_set(FALSE);

    SDK_DelayAtLeastUs(150000, CLOCK_GetFreq(kCLOCK_CpuClk));

    /* Initialize PHY for EP. */
    phy8201Config.resource = &g_xether_board.phy_resource[XETHER_NETC_PORT_ETH4];
    phy8201Config.phyAddr  = BOARD_EP0_PHY_ADDR;

    result = xether_netc_phy_init(XETHER_NETC_PORT_ETH4, &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = xether_netc_phy_setup_rtl8201(&g_xether_board.phy_handle[XETHER_NETC_PORT_ETH4]);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize PHY for switch port0. */
    phy8201Config.resource = &g_xether_board.phy_resource[XETHER_NETC_PORT_ETH0];
    phy8201Config.phyAddr  = BOARD_SWT_PORT0_PHY_ADDR;

    result = xether_netc_phy_init(XETHER_NETC_PORT_ETH0, &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = xether_netc_phy_setup_rtl8201(&g_xether_board.phy_handle[XETHER_NETC_PORT_ETH0]);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize PHY for switch port1. */
    phy8211Config.resource = &g_xether_board.phy_resource[XETHER_NETC_PORT_ETH1];
    phy8211Config.phyAddr  = BOARD_SWT_PORT1_PHY_ADDR;

    result = xether_netc_phy_init(XETHER_NETC_PORT_ETH1, &phy8211Config);
    if (result != kStatus_Success)
    {
        return result;
    }

	/* Initialize PHY for switch port2. */
	phy8211Config.resource = &g_xether_board.phy_resource[XETHER_NETC_PORT_ETH2];
	phy8211Config.phyAddr  = BOARD_SWT_PORT2_PHY_ADDR;

	result = xether_netc_phy_init(XETHER_NETC_PORT_ETH2, &phy8211Config);
	if (result != kStatus_Success)
	{
		return result;
	}

	/* Initialize PHY for switch port3. */
	phy8211Config.resource = &g_xether_board.phy_resource[XETHER_NETC_PORT_ETH3];
	phy8211Config.phyAddr  = BOARD_SWT_PORT3_PHY_ADDR;

	result = xether_netc_phy_init(XETHER_NETC_PORT_ETH3, &phy8211Config);
	if (result != kStatus_Success)
	{
		return result;
	}

    return result;
}


static void xether_netc_ep0_msgintr_callback(MSGINTR_Type *base, uint8_t channel, uint32_t pendingIntr)
{
	/* NETC Transmit interrupt */
	if ((pendingIntr & (1U << NETC0_TX_INTR_MSG_DATA)) != 0U)
	{
		EP_CleanTxIntrFlags(&g_xether_board.ep_handle[XETHER_NETC_EP0], 1, 0);
	}

	/* NETC Receive interrupt */
	if ((pendingIntr & (1U << NETC0_RX_INTR_MSG_DATA)) != 0U)
	{
		EP_CleanRxIntrFlags(&g_xether_board.ep_handle[XETHER_NETC_EP0], 1);
	}
}

static status_t xether_netc_ep0_reclaim_callback(ep_handle_t *handle, uint8_t ring, netc_tx_frame_info_t *frameInfo, void *userData)
{
	return (kStatus_Success);
}

static bool_t xether_netc_init(ep_handle_t *handle, uint8_t *mac_addr, const ep_config_t *ep_config, const netc_bdr_config_t *bdr_config)
{
}

static void xether_netc_deinit(uint8_t index)
{
}

static bool_t xether_netc_ep0_open(const xether_config_t *config)
{
	status_t result                  = kStatus_Success;

	if (config != NULL) {
		netc_rx_bdr_config_t	rxBdrConfig = {0};
		netc_tx_bdr_config_t	txBdrConfig = {0};
		netc_bdr_config_t		bdrConfig = {.rxBdrConfig = &rxBdrConfig, .txBdrConfig = &txBdrConfig};
		bool_t					link = false;
		netc_msix_entry_t		msix_entry[XETHER_NETC_MSIX_ENTRYID_MAX];
		netc_hw_mii_mode_t		phy_mode;
		netc_hw_mii_speed_t		phy_speed;
		netc_hw_mii_duplex_t	phy_duplex;
		ep_config_t				ep_config;
		uint32_t				msg_addr;
		uint32_t				length;

		/* MSIX and interrupt configuration. */
		MSGINTR_Init(XETHER_NETC_EP0_MSGINTR, &xether_netc_ep0_msgintr_callback);
		msg_addr = MSGINTR_GetIntrSelectAddr(XETHER_NETC0_MSGINTR, 0);

		msix_entry[XETHER_NETC_MSIX_ENTRYID_RX].control = kNETC_MsixIntrMaskBit;
		msix_entry[XETHER_NETC_MSIX_ENTRYID_RX].msgAddr = msg_addr;
		msix_entry[XETHER_NETC_MSIX_ENTRYID_RX].msgData = NETC0_RX_INTR_MSG_DATA;

		msix_entry[XETHER_NETC_MSIX_ENTRYID_TX].control = kNETC_MsixIntrMaskBit;
		msix_entry[XETHER_NETC_MSIX_ENTRYID_TX].msgAddr = msg_addr;
		msix_entry[XETHER_NETC_MSIX_ENTRYID_TX].msgData = NETC0_TX_INTR_MSG_DATA;

		/* BD ring configuration. */
		bdrConfig.rxBdrConfig[0].bdArray       = &g_xether_netc0_rxbuff_descriptor[0][0];
		bdrConfig.rxBdrConfig[0].len           = NETC_EP0_RXBD_NUM;
		bdrConfig.rxBdrConfig[0].buffAddrArray = &g_xether_netc0_rxbuff_addr_array[0][0];
		bdrConfig.rxBdrConfig[0].buffSize      = NETC_EP0_RXBUFF_SIZE_ALIGN;
		bdrConfig.rxBdrConfig[0].msixEntryIdx  = NETC_EP0_RX_MSIX_ENTRY_IDX;
		bdrConfig.rxBdrConfig[0].extendDescEn  = false;
		bdrConfig.rxBdrConfig[0].enThresIntr   = true;
		bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
		bdrConfig.rxBdrConfig[0].intrThreshold = 1;

		bdrConfig.txBdrConfig[0].bdArray      = &g_xether_netc0_txbuff_descriptor[0][0];
		bdrConfig.txBdrConfig[0].len          = NETC_EP0_TXBD_NUM;
		bdrConfig.txBdrConfig[0].dirtyArray   = &g_xether_netc0_txdirty[0][0];
		bdrConfig.txBdrConfig[0].msixEntryIdx = NETC_EP0_TX_MSIX_ENTRY_IDX;
		bdrConfig.txBdrConfig[0].enIntr       = true;

		/* Wait PHY link up. */
		do
		{
			result = APP_PHY_GetLinkStatus(index, &link);
		} while ((result != kStatus_Success) || (!link));
		result = APP_PHY_GetLinkModeSpeedDuplex(index, &phyMode, &phySpeed, &phyDuplex);
		if (result != kStatus_Success)
		{
			return result;
		}

		/* Wait a moment for PHY status to be stable. */
		SDK_DelayAtLeastUs(PHY_STABILITY_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

		/* Endpoint configuration. */
		EP_GetDefaultConfig(&ep_config);
		ep_config.si                    = kNETC_ENETC0PSI0;
		ep_config.siConfig.txRingUse    = 1;
		ep_config.siConfig.rxRingUse    = 1;
		ep_config.reclaimCallback       = xether_netc0_reclaim_callback;
		ep_config.userData				=
		ep_config.msixEntry             = &msix_entry[0];
		ep_config.entryNum              = 2;
		ep_config.port.ethMac.miiMode   = phyMode;
		ep_config.port.ethMac.miiSpeed  = phySpeed;
		ep_config.port.ethMac.miiDuplex = phyDuplex;

		#if (defined(FSL_FEATURE_NETC_HAS_ERRATA_052167) && FSL_FEATURE_NETC_HAS_ERRATA_052167)
		/* ERR052167: Actual MAC Tx IPG is longer than configured when transmitting back-to-back packets in MII half duplex
		mode by approximately 15 extra bytes. For example, when configured for IPG=12, the actual IPG will be
		approximately 27. The net result is that maximum throughput will be reduced also in the absence of half-duplex
		collision/retry events. When using MII protocol, using full-duplex mode is recommended instead of half-duplex. If
		using MII half-duplex mode, additional bandwidth loss should be expected and accounted for due to extended IPG. */
		assert(!((phyMode == kNETC_MiiMode) && (phyDuplex == kNETC_MiiHalfDuplex)));
		#endif

		result = EP_Init(&g_xether_board.ep_handle[XETHER_NETC_EP0], &g_macAddr[0], &ep_config, &bdrConfig);

		/* Unmask MSIX message interrupt. */
		EP_MsixSetEntryMask(&g_ep_handle, XETHER_NETC_MSIX_ENTRYID_TX, false);
		EP_MsixSetEntryMask(&g_ep_handle, XETHER_NETC_MSIX_ENTRYID_RX, false);

	}


	return (result == kStatus_Success);
}

static void xether_netc_ep0_close(void)
{
	xether_netc_ep_deinit(0);
}

static struct pbuf *xether_netc_ep0_recv_packet_get(void)
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

static bool_t xether_netc_ep0_send_packet_set(struct pbuf *p)
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
		result = EP_SendFrame(&g_xether_board.ep_handle[XETHER_NETC_EP0], 0, p->tot_len, 0, false, NULL);
	}
	while (result == kStatus_ENET_TxFrameBusy);

	return (TRUE);
}

static bool_t xether_netc_ep0_link_status_update(void)
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
    /* EP and Switch port 0 use RMII interface. */
    NETC_SocSetMiiMode(kNETC_SocLinkEp0, kNETC_RmiiMode);
    NETC_SocSetMiiMode(kNETC_SocLinkSwitchPort0, kNETC_RmiiMode);

    /* Switch port 1~3 use RGMII interface. */
    NETC_SocSetMiiMode(kNETC_SocLinkSwitchPort1, kNETC_RgmiiMode);
    NETC_SocSetMiiMode(kNETC_SocLinkSwitchPort2, kNETC_RgmiiMode);
    NETC_SocSetMiiMode(kNETC_SocLinkSwitchPort3, kNETC_RgmiiMode);

    /* Output reference clock for RMII interface. */
    NETC_SocSetRmiiRefClk(kNETC_SocLinkEp0, true);
    NETC_SocSetRmiiRefClk(kNETC_SocLinkSwitchPort0, true);

    /* Unlock the IERB. It will warm reset whole NETC. */
    if (NETC_IERBUnlock() == kStatus_Success)
    {
        while (!NETC_IERBIsUnlockOver())
        {
        }
    }

    /* Set the access attribute, otherwise MSIX access will be blocked. */
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR &= ~(7U << 27);
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR |= (1U << 27);

    /* Set PHY address in IERB to use MAC port MDIO, otherwise the access will be blocked. */
    NETC_SocSetLinkAddr(kNETC_SocLinkEp0, BOARD_EP0_PHY_ADDR);
    NETC_SocSetLinkAddr(kNETC_SocLinkSwitchPort0, BOARD_SWT_PORT0_PHY_ADDR);
    NETC_SocSetLinkAddr(kNETC_SocLinkSwitchPort1, BOARD_SWT_PORT1_PHY_ADDR);
    NETC_SocSetLinkAddr(kNETC_SocLinkSwitchPort2, BOARD_SWT_PORT2_PHY_ADDR);
    NETC_SocSetLinkAddr(kNETC_SocLinkSwitchPort3, BOARD_SWT_PORT3_PHY_ADDR);

    /* Lock the IERB. */
    assert(NETC_IERBLock() == kStatus_Success);
    while (!NETC_IERBIsLockOver())
    {
    }

}

static inline void xether_deinit_board(void)
{
}

XETHERNET_DEVICE_LIST_BEGIN()
  XETHERNET_DEVICE_LIST_ITEM(xether_netc_ep0),
XETHERNET_DEVICE_LIST_END()

#endif /* XETHER_BOARD_H_ */
