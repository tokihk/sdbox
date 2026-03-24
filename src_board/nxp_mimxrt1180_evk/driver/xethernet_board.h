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
#include "netc/fsl_netc_endpoint.h"

#include "fsl_phyrtl8201.h"
#include "fsl_phyrtl8211f.h"


#define NETC_EP_FRAME_LEN_MAX			(ENET_FRAME_MAX_FRAMELEN + ENET_FRAME_VLAN_TAGLEN)

#define ENET1G_TXBUFF_SIZE				(ENET1G_FRAME_LEN_MAX)
#define ENET1G_RXBUFF_SIZE				(ENET1G_FRAME_LEN_MAX)

#define ENET1G_TXBD_NUM					(24)
#define ENET1G_RXBD_NUM					(24)

#define ENET1G_RXBUFF_NUM				(ENET1G_RXBD_NUM * 2)

#define ENET1G_MAX_BUFFERS_PER_FRAME	((ENET1G_FRAME_LEN_MAX / ENET1G_RXBUFF_SIZE) + ((ENET1G_FRAME_LEN_MAX % ENET1G_RXBUFF_SIZE == 0) ? 0 : 1))

#define NETC_EP_BUFF_SIZE_ALIGN			64U
#define NETC_EP_BD_ALIGN				128U

#define NETC_EP_RXRING_NUM				3U
#define NETC_EP_RXBD_NUM				8U
#define NETC_EP_RXBUFF_SIZE				1522U
#define NETC_EP_RXBUFF_SIZE_ALIGN		SDK_SIZEALIGN(NETC_EP_RXBUFF_SIZE, NETC_EP_BUFF_SIZE_ALIGN)

#define NETC_EP_TXRING_NUM				3U
#define NETC_EP_TXFRAME_NUM				20U
#define NETC_EP_TXBD_NUM				8U
#define NETC_EP_TXBUFF_SIZE				1522U
#define NETC_EP_TXBUFF_SIZE_ALIGN		SDK_SIZEALIGN(NETC_EP_TXBUFF_SIZE, NETC_EP_BUFF_SIZE_ALIGN)


typedef enum xether_netc_ep_id
{
	XETHER_NETC_EP0,
	XETHER_NETC_EP1,
	XETHER_NETC_EP2,
	XETHER_NETC_EP3,
	XETHER_NETC_EP4,

	XETHER_NEC_EP_NUM
} xether_netc_ep_id_t;


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

typedef struct xether_ep_status
{
	phy_speed_t					last_speed;
	phy_duplex_t				last_duplex;
	bool_t						last_link_up;
} xether_ep_status_t;

typedef struct xether_ep_handle
{
	phy_handle_t				phy_handle;
	phy_rtl8211f_resource_t		phy_resource;

	netc_hw_si_idx_t			hw_si_id;

	xether_ep_status_t			status;
} xether_ep_handle_t;

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

	phy_handle_t				phy_handle[XETHER_NEC_EP_NUM];
	phy_speed_t					last_speed;
	phy_duplex_t				last_duplex;
	bool_t						last_link_up;
} g_xether_enet1g;


typedef uint8_t										netc_ep_rx_buffer_t[NETC_EP_RXBUFF_SIZE_ALIGN];
typedef uint8_t										netc_ep_tx_buffer_t[NETC_EP_TXBUFF_SIZE_ALIGN];

AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t			g_rxBuffDescrip[NETC_EP_RXRING_NUM][NETC_EP_RXBD_NUM], NETC_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_ep_rx_buffer_t	g_rxDataBuff[NETC_EP_RXRING_NUM][NETC_EP_RXBD_NUM], NETC_EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t				g_rxFrame[NETC_EP_RXBUFF_SIZE_ALIGN], NETC_EP_BUFF_SIZE_ALIGN);

AT_NONCACHEABLE_SECTION_ALIGN(static netc_ep_tx_buffer_t	g_txFrame[NETC_EP_TXBUFF_SIZE], NETC_EP_BUFF_SIZE_ALIGN);

AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t			g_mgmtTxBuffDescrip[NETC_EP_TXBD_NUM], NETC_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t			g_cmdBuffDescrip[NETC_EP_TXBD_NUM], NETC_EP_BD_ALIGN);


static uint64_t												g_rxBuffAddrArray[NETC_EP_RXRING_NUM][NETC_EP_RXBD_NUM];
static netc_tx_frame_info_t									g_mgmtTxDirty[NETC_EP_TXBD_NUM];
static netc_tx_frame_info_t									g_mgmtTxFrameInfo;

AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t	g_txBuffDescrip[NETC_EP_TXRING_NUM][NETC_EP_TXBD_NUM], NETC_EP_BD_ALIGN);
static netc_tx_frame_info_t							g_txDirty[NETC_EP_TXRING_NUM][NETC_EP_TXBD_NUM];


/* PHY operation. */
static netc_mdio_handle_t			g_mdio_handle;
static phy_rtl8201_resource_t		g_phy_rtl8201_resource;

static const phy_operations_t		g_app_phy_rtl8201_ops =
{
		.phyInit            = APP_PHY_RTL8201_Init,
		.phyWrite           = PHY_RTL8201_Write,
		.phyRead            = PHY_RTL8201_Read,
		.getAutoNegoStatus  = PHY_RTL8201_GetAutoNegotiationStatus,
		.getLinkStatus      = PHY_RTL8201_GetLinkStatus,
		.getLinkSpeedDuplex = PHY_RTL8201_GetLinkSpeedDuplex,
		.setLinkSpeedDuplex = PHY_RTL8201_SetLinkSpeedDuplex,
		.enableLoopback     = PHY_RTL8201_EnableLoopback,
		.enableLinkInterrupt= PHY_RTL8201_EnableLinkInterrupt,
		.clearInterrupt     = PHY_RTL8201_ClearInterrupt
};


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

static status_t APP_Phy8201SetUp(phy_handle_t *handle)
{
    status_t result;
    uint16_t data;

    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 7);
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
    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 0);

    return result;
}

static status_t APP_PHY_SetPort(uint32_t port, phy_config_t *phyConfig)
{
    status_t result = kStatus_Success;

#ifdef EXAMPLE_PHY_USE_PORT_MDIO
    s_phy_resource[port].write = APP_PMDIOWrite;
    s_phy_resource[port].read  = APP_PMDIORead;
#else
    s_phy_resource[port].write = APP_EMDIOWrite;
    s_phy_resource[port].read  = APP_EMDIORead;
#endif
    result = PHY_Init(&s_phy_handle[port], phyConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    return PHY_EnableLoopback(&s_phy_handle[port], kPHY_LocalLoop, phyConfig->speed, true);
}

status_t APP_PHY_Init(void)
{
    status_t result            = kStatus_Success;
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
    phy8201Config.resource = &s_phy_resource[EXAMPLE_EP0_PORT];
    phy8201Config.phyAddr  = BOARD_EP0_PHY_ADDR;
    result                 = APP_PHY_SetPort(EXAMPLE_EP0_PORT, &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = APP_Phy8201SetUp(&s_phy_handle[EXAMPLE_EP0_PORT]);
    if (result != kStatus_Success)
    {
        return result;
    }
#if defined(EXAMPLE_PORT_USE_100M_HALF_DUPLEX_MODE)
    uint16_t phyRegValue;
    (void)PHY_Write(&s_phy_handle[EXAMPLE_EP0_PORT], 0x1F, 7);
    (void)PHY_Read(&s_phy_handle[EXAMPLE_EP0_PORT], 20, &phyRegValue);
    (void)PHY_Write(&s_phy_handle[EXAMPLE_EP0_PORT], 20, (phyRegValue | 0x900U));
    (void)PHY_Write(&s_phy_handle[EXAMPLE_EP0_PORT], 0x1F, 0);
#endif

    /* Initialize PHY for switch port0. */
    phy8201Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT0];
    phy8201Config.phyAddr  = BOARD_SWT_PORT0_PHY_ADDR;
    result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT0, &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = APP_Phy8201SetUp(&s_phy_handle[EXAMPLE_SWT_PORT0]);
    if (result != kStatus_Success)
    {
        return result;
    }
#if defined(EXAMPLE_PORT_USE_100M_HALF_DUPLEX_MODE)
    (void)PHY_Write(&s_phy_handle[EXAMPLE_SWT_PORT0], 0x1F, 7);
    (void)PHY_Read(&s_phy_handle[EXAMPLE_SWT_PORT0], 20, &phyRegValue);
    (void)PHY_Write(&s_phy_handle[EXAMPLE_SWT_PORT0], 20, (phyRegValue | 0x900U));
    (void)PHY_Write(&s_phy_handle[EXAMPLE_SWT_PORT0], 0x1F, 0);
#endif

    /* Initialize PHY for switch port1. */
    phy8211Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT1];
    phy8211Config.phyAddr  = BOARD_SWT_PORT1_PHY_ADDR;
    result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT1, &phy8211Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    if (((1U << 2) & EXAMPLE_SWT_USED_PORT_BITMAP) != 0U)
    {
        /* Initialize PHY for switch port2. */
        phy8211Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT2];
        phy8211Config.phyAddr  = BOARD_SWT_PORT2_PHY_ADDR;
        result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT2, &phy8211Config);
        if (result != kStatus_Success)
        {
            return result;
        }
    }

    if (((1U << 3) & EXAMPLE_SWT_USED_PORT_BITMAP) != 0U)
    {
        /* Initialize PHY for switch port3. */
        phy8211Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT3];
        phy8211Config.phyAddr  = BOARD_SWT_PORT3_PHY_ADDR;
        result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT3, &phy8211Config);
        if (result != kStatus_Success)
        {
            return result;
        }
    }

    return result;
}

status_t APP_PHY_GetLinkStatus(uint32_t port, bool *link)
{
    return PHY_GetLinkStatus(&s_phy_handle[port], link);
}


status_t APP_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = EXAMPLE_NETC_FREQ,
    };

#ifdef EXAMPLE_PHY_USE_PORT_MDIO
    /* Usually should call EP_Init/SWT_Init then init port MDIO, here just an quick enablement example. */
    NETC_F2_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);
    NETC_F3_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    for (int i = 0U; i < 5U; i++)
    {
        mdioConfig.mdio.port = (netc_hw_eth_port_idx_t)((uint32_t)kNETC_ENETC0EthPort + i);
        result               = NETC_MDIOInit(&s_mdio_handle[i], &mdioConfig);
        if (result != kStatus_Success)
        {
            return result;
        }
    }
#else
    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }
#endif

    return result;
}

static status_t APP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t APP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

static void xether_enet1g_mdio_init(void)
{
	CLOCK_EnableClock(s_enetClock[ENET_GetInstance(ENET_1G)]);

	ENET_SetSMI(ENET_1G, CLOCK_GetRootClockFreq(kCLOCK_Root_Bus), false);
}

static status_t xether_netc_ep_mdio_write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(ENET_1G, phyAddr, regAddr, data);
}

static status_t xether_netc_ep_mdio_read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(ENET_1G, phyAddr, regAddr, pData);
}

static bool_t xether_netc_ep_phy_init(void)
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

static bool_t xether_netc_ep_init(uint8_t index, const xether_config_t *config)
{
	bool_t					success = FALSE;
	ep_config_t				ep_config;
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

	/* Endpoint Config */
	EP_GetDefaultConfig(&ep_config);
	ep_config.si                    = g_siIndex[index];
	ep_config.siConfig.txRingUse    = 1;
	ep_config.siConfig.rxRingUse    = 1;
	ep_config.reclaimCallback       = APP_ReclaimCallback;
	ep_config.msixEntry             = &msixEntry[0];
	ep_config.entryNum              = 2;
	ep_config.port.ethMac.miiMode   = phyMode;
	ep_config.port.ethMac.miiSpeed  = phySpeed;
	ep_config.port.ethMac.miiDuplex = phyDuplex;

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
		result = EP_Init(
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

static void xether_netc_ep_deinit(uint8_t index)
{
}

static bool_t xether_netc_ep0_open(const xether_config_t *config)
{
	bool_t open_ok = FALSE;

	open_ok = xether_netc_ep_init(0, config);

	return (open_ok);
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
		result = ENET_SendFrame(ENET_1G, &g_xether_enet1g.enet_handle, pucBuffer, p->tot_len, 0, false, NULL);
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
