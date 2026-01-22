/*
 * xnetif.c
 *
 *  Created on: 2024/10/15
 *      Author: Hitoshi Kono
 */

#include <config/xconfig_const.h>
#include "xnetif.h"

#include "driver/xethernet.h"

#include "core/xmalloc.h"
#include "core/xtask.h"

#include "lwip/init.h"
#include "lwip/etharp.h"
#include "netif/ethernet.h"


#define XNETIF_LINK_CHECK_IVAL					(100)


typedef struct xnetif_object
{
	struct netif				lwip_netif;

	xnetif_config_t				config;
	xnetif_status_t				status;

	uint32_t					link_check_tick_prev;

	bool_t						is_lwip_opened;
} xnetif_object_t;

static struct
{
	uint8_t						netif_count;

	bool_t						is_init;
} g_xnetif;


static void xnetif_lwip_close(xnetif_object_t *obj)
{
	netif_remove(&(obj->lwip_netif));
}

static bool_t xnetif_lwip_open(xnetif_object_t *obj, netif_init_fn init_func)
{
	struct netif *lwip_netif = NULL;

	if (obj->is_lwip_opened)
	{
		xnetif_lwip_close(obj);
	}

	lwip_netif = netif_add(
			&(obj->lwip_netif),
			&(obj->config.ipv4_addr),
			&(obj->config.ipv4_netmask),
			&(obj->config.ipv4_gateway),
			obj,
			init_func,
			&ethernet_input);

	if (lwip_netif != NULL)
	{
		obj->is_lwip_opened = TRUE;
	}

	return (lwip_netif != NULL);
}

static void xnetif_lwip_link_update(xnetif_object_t *obj, bool_t linkup)
{
	if (linkup)
	{
		netif_set_up(&obj->lwip_netif);
		netif_set_link_up(&obj->lwip_netif);
	}
	else
	{
		netif_set_down(&obj->lwip_netif);
		netif_set_link_down(&obj->lwip_netif);
	}
}

static void xnetif_lwip_recv(xnetif_object_t *obj, struct pbuf *pkt)
{
	if (   (obj->lwip_netif.input != NULL)
		&& (obj->lwip_netif.input)(pkt, &(obj->lwip_netif)) != ERR_OK)
	{
		pbuf_free(pkt);
	}
}

static void xnetif_eth0_link_poll(xnetif_object_t *obj)
{
	uint32_t tick_curr;

	tick_curr = xtask_tick_get();
	if (xtask_tick_erapsed_msec_get(obj->link_check_tick_prev, tick_curr) >= XNETIF_LINK_CHECK_IVAL)
	{
		xnetif_lwip_link_update(obj, xether_link_status_update(0));

		obj->link_check_tick_prev = tick_curr;
	}
}

static void xnetif_eth0_recv_poll(xnetif_object_t *obj)
{
	struct pbuf *recv_packet;

	while ((recv_packet = xether_recv_packet_get(0)) != NULL)
	{
		xnetif_lwip_recv(obj, recv_packet);
	}
}

static err_t xnetif_eth0_lwip_linkoutput(struct netif *netif, struct pbuf *p)
{
	err_t result = ERR_OK;

	/* Initiate transfer. */
#if ETH_PAD_SIZE
	pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

	if (!xether_send_packet_set(0, p))
	{
		result = ERR_IF;
	}

#if ETH_PAD_SIZE
	pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

	LINK_STATS_INC(link.xmit);

	return (result);
}

static err_t xnetif_eth0_lwip_netif_init(struct netif *netif)
{
	xnetif_object_t *obj = (xnetif_object_t *)netif->state;
	xether_config_t eth_config = {0};

	LWIP_ASSERT("netif != NULL", (netif != NULL));

	eth_config.mac_addr = obj->config.mac_addr;

	if (!xether_open(0, &eth_config))
	{
		return (ERR_IF);
	}

#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

	/*
	* Initialize the snmp variables and counters inside the struct netif.
	* The last argument should be replaced with your link speed, in units
	* of bits per second.
	*/
	// MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

	netif->name[0] = 'e';
	netif->name[1] = (char)('0' + g_xnetif.netif_count++);
	/* We directly use etharp_output() here to save a function call.
	* You can instead declare your own function an call etharp_output()
	* from it if you have to do some checks before sending (e.g. if link
	* is available...) */

#if LWIP_IPV4
#if LWIP_ARP || LWIP_ETHERNET
#if LWIP_ARP
	netif->output = etharp_output;
#else
	/* The user should write its own code in low_level_output_arp_off function */
	netif->output = low_level_output_arp_off;
#endif /* LWIP_ARP */
#endif /* LWIP_ARP || LWIP_ETHERNET */
#endif /* LWIP_IPV4 */

#if LWIP_IPV6
	netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */

	netif->linkoutput = xnetif_eth0_lwip_linkoutput;

#if LWIP_ARP || LWIP_ETHERNET

	/* MACアドレス設定 */
	netif->hwaddr_len = XMIN(XCOUNTOF(obj->config.mac_addr.addr), XCOUNTOF(netif->hwaddr));
	memcpy(netif->hwaddr, obj->config.mac_addr.addr, netif->hwaddr_len);

	/* maximum transfer unit */
	netif->mtu = XMIN(1500, obj->config.mtu);
	if (netif->mtu == 0)
	{
		netif->mtu = 1500;
	}

	/* Accept broadcast address and ARP traffic */
	/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
#if LWIP_ARP
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
#else
	netif->flags |= NETIF_FLAG_BROADCAST;
#endif /* LWIP_ARP */

#endif /* LWIP_ARP || LWIP_ETHERNET */

	return (ERR_OK);
}

static void xnetif_eth0_close(xnetif_object_t *obj)
{
	if (obj->status.is_opened)
	{
		xnetif_lwip_close(obj);
	}
}

bool_t xnetif_eth0_open(xnetif_object_t *obj, const xnetif_config_t *config)
{
	bool_t is_success = FALSE;

	if (   (obj != NULL)
		&& (config != NULL)
		&& (!obj->status.is_opened)
	) {
		obj->config = *config;
		obj->link_check_tick_prev = xtask_tick_get();

		if (!xnetif_lwip_open(obj, xnetif_eth0_lwip_netif_init))
		{
			/* LWIP初期化失敗 */
		}
		else
		{
			/* 初期化成功 */
			obj->status.is_opened = TRUE;
			is_success = TRUE;
		}

		if (!is_success)
		{
			xnetif_eth0_close(obj);
		}
	}

	return (is_success);
}

static void xnetif_eth0_poll(xnetif_object_t *obj)
{
	xnetif_eth0_link_poll(obj);
	xnetif_eth0_recv_poll(obj);
}

static const struct xnetif_info
{
	const char *				name;
	bool_t						(*hal_open)(xnetif_object_t *obj, const xnetif_config_t *config);
	void						(*hal_close)(xnetif_object_t *obj);
	void						(*hal_poll)(xnetif_object_t *obj);
} XNETIF_INFO_TABLE[] =
{
	/* eth0 */
	{
		.name = "eth0",
		.hal_open  = xnetif_eth0_open,
		.hal_close = xnetif_eth0_close,
		.hal_poll  = xnetif_eth0_poll,
	},
};


xnetif_object_t					g_xnetif_objs[XCOUNTOF(XNETIF_INFO_TABLE)];


void xnetif_init(void)
{
	lwip_init();
}

void xnetif_deinit(void)
{

}

void xnetif_poll(void)
{
	uint8_t id;

	for (id = 0; id < XCOUNTOF(g_xnetif_objs); id++)
	{
		if (g_xnetif_objs[id].status.is_opened)
		{
			(XNETIF_INFO_TABLE[id].hal_poll)(&g_xnetif_objs[id]);
		}
	}
}

bool_t xnetif_open(uint16_t id, const xnetif_config_t *config)
{
	bool_t is_success = FALSE;

	if (   (id < XCOUNTOF(g_xnetif_objs))
		&& (config != NULL)
	) {
		xnetif_object_t *obj = &g_xnetif_objs[id];

		if (obj->status.is_opened)
		{
			/* Error: 既にオープン済み */
		}
		else if (!(XNETIF_INFO_TABLE[id].hal_open)(obj, config))
		{
			/* Error: オープン失敗 */
		}
		else
		{
			/* Success */
			obj->status.is_opened = TRUE;

			is_success = TRUE;
		}
	}

	return (is_success);
}

void xnetif_close(uint16_t id)
{
	if (id < XCOUNTOF(g_xnetif_objs))
	{
		xnetif_object_t *obj = &g_xnetif_objs[id];

		if (obj->status.is_opened)
		{
			(XNETIF_INFO_TABLE[id].hal_close)(obj);

			/* Status clear */
			memset(&obj->status, 0, sizeof(obj->status));
		}
	}
}

bool_t xnetif_config_get(uint16_t id, xnetif_config_t *config)
{
	bool_t get_ok = FALSE;

	if (   (id < XCOUNTOF(g_xnetif_objs))
		&& (config != NULL)
	) {
		xnetif_object_t *obj = &g_xnetif_objs[id];

		*config = obj->config;
	}

	return (get_ok);
}

void xnetif_status_reset(uint16_t id)
{
	if (id < XCOUNTOF(g_xnetif_objs))
	{
		xnetif_object_t *obj = &g_xnetif_objs[id];

		obj->status.rx_packet_count = 0;
		obj->status.tx_packet_count = 0;
	}
}

bool_t xnetif_status_get(uint16_t id, xnetif_status_t *status)
{
	bool_t get_ok = FALSE;

	if (   (id < XCOUNTOF(g_xnetif_objs))
		&& (status != NULL)
	) {
		xnetif_object_t *obj = &g_xnetif_objs[id];

		*status = obj->status;
	}

	return (get_ok);
}
