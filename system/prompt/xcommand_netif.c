/*
 * xcommand_netif.c
 *
 *  Created on: 2024/10/08
 *      Author: Hitoshi Kono
 */

#include "xcommand_netif.h"
#include "xcommand.h"
#include "xprompt.h"

#include "driver/xethernet.h"
#include "driver/xnetif.h"

#include "lwip/prot/ethernet.h"


static bool_t xcommand_text2macaddr(const char *text, struct eth_addr *macaddr)
{
	bool_t success = FALSE;

	if ((text != NULL) && (macaddr != NULL)) {
		char	HEXCODE[] = "0123456789ABCDEF";

		uint8_t	macaddr_index = 0;
		uint8_t	macaddr_value = 0;
		uint8_t	value_part_count = 0;
		char *	value_part;

		while (*text != '\0') {
			if (*text == ':') {
				if (value_part_count == 2) {
					macaddr->addr[macaddr_index++] = macaddr_value;
					macaddr_value = 0;
					value_part_count = 0;
				} else {
					/* フォーマットエラー */
					break;
				}

			} else if (value_part_count >= 2) {
				/* フォーマットエラー */
				break;

			} else {
				value_part = strchr(HEXCODE, *text);
				if (value_part != NULL) {
					macaddr_value <<= 4;
					macaddr_value += (uint8_t)(value_part - &HEXCODE[0]);
					value_part_count++;

				} else {
					/* フォーマットエラー */
					break;
				}
			}
			text++;
		}

		/* 収集した値の数が正しければ成功 */
		if ((macaddr_index == (XCOUNTOF(macaddr->addr) - 1)) && (value_part_count == 2)) {
			macaddr->addr[macaddr_index++] = macaddr_value;
			success = TRUE;
		}
	}

	return (success);
}


/* --------------------------------------------------------
 * netif start [no] [hwaddr]
 * ----------------------------------------------------- */

const char XCOMMAND_NETIF_START_HELP[] = {
		"Network interface start.\n"
		"  netif start [id] {hwaddr=} {hwaddr=} {ipaddr=} {netmask=} {gw=} \n"
		"    id      : Interface ID\n"
		"    hwaddr  : MAC address (ex. hwaddr=8E:80:10:00:00:00)\n"
		"    ipaddr  : Static IPv4 address (ex. ipaddr=192.168.100.100)\n"
		"    netmask : Static netmask (ex. netmask=255.255.255.0)\n"
		"    gw      : Static Default Gateway (ex. gw=192.168.100.0)\n"
};

static bool_t xcommand_netif_start(xcommand_callstack_t *cs)
{
	bool_t					success = TRUE;
	const char *			argv_text;
	xcommand_arg_t			arg;

	uint8_t					id = 0;
	xnetif_config_t			config = {0};

	/* Default setting */
	config.mac_addr.addr[0] = 0x00;
	config.mac_addr.addr[1] = 0x80;
	config.mac_addr.addr[2] = 0xE1;
	config.mac_addr.addr[3] = 0x00;
	config.mac_addr.addr[4] = 0x00;
	config.mac_addr.addr[5] = 0x00 + id;

	IP4_ADDR(&config.ipv4_addr,    192, 168, 100, 100 + id);
	IP4_ADDR(&config.ipv4_netmask, 255, 255, 255, 0);
	IP4_ADDR(&config.ipv4_gateway, 192, 168, 100, 0);

	/* id */
	if (xcommand_arg_pop_text(cs, &argv_text))
	{
		id = xcommand_text2uint(argv_text);
	}

	while (xcommand_arg_pop(cs, &arg))
	{
		/* macaddr */
		if (strcmp(arg.name, "macaddr") == 0)
		{
			if (!xcommand_text2macaddr(arg.value, &config.mac_addr))
			{
				/* フォーマットエラー */
				success = FALSE;
			}
		}

		/* ipaddr */
		else if (strcmp(arg.name, "ipaddr") == 0)
		{
			if (!ip4addr_aton(arg.value, &config.ipv4_addr))
			{
				/* フォーマットエラー */
				success = FALSE;
			}
		}

		/* netmask */
		else if (strcmp(arg.name, "netmask") == 0)
		{
			if (!ip4addr_aton(arg.value, &config.ipv4_netmask))
			{
				/* フォーマットエラー */
				success = FALSE;
			}
		}

		/* gw */
		else if (strcmp(arg.name, "gw") == 0)
		{
			if (!ip4addr_aton(arg.value, &config.ipv4_gateway))
			{
				/* フォーマットエラー */
				success = FALSE;
			}
		}
	}

	if (success)
	{
		xnetif_close(id);

		if (!xnetif_open(id, &config))
		{

		}
		else
		{
			success = TRUE;
		}
	}

	return (success);
}

/* --------------------------------------------------------
 * netif stop
 * ----------------------------------------------------- */

static bool_t xcommand_netif_stop(xcommand_callstack_t *cs)
{
	return (TRUE);
}

/* --------------------------------------------------------
 * netif list
 * ----------------------------------------------------- */

static bool_t xcommand_netif_list(xcommand_callstack_t *cs)
{
	uint16_t			id_max;
	uint16_t			id;
	xnetif_config_t		config;
	xnetif_status_t		status;

	id_max = xether_device_num_get();
	for (id = 0; id < id_max; id++) {
		if (   (xnetif_config_get(id, &config))
			&& (xnetif_status_get(id, &status))
		) {
			const char *output;

			if (status.is_opened)
			{
				output = (status.is_linkup) ? ("link up") : ("link down");
			}
			else
			{
				output = "disable";
			}

			xprompt_printf(
				cs->prompt_attr,
				"eth%d: %s",
				id,
				output
			);

			if (status.is_opened)
			{
				xprompt_printf(
					cs->prompt_attr,
					"  hwaddr %02X:%02X:%02X:%02X:%02X:%02X",
					config.mac_addr.addr[0],
					config.mac_addr.addr[1],
					config.mac_addr.addr[2],
					config.mac_addr.addr[3],
					config.mac_addr.addr[4],
					config.mac_addr.addr[5]
				);

				xprompt_printf(
					cs->prompt_attr,
					"  ipaddr %d.%d.%d.%d  netmask %d.%d.%d.%d",
					ip4_addr1(&config.ipv4_addr),
					ip4_addr2(&config.ipv4_addr),
					ip4_addr3(&config.ipv4_addr),
					ip4_addr4(&config.ipv4_addr),
					ip4_addr1(&config.ipv4_netmask),
					ip4_addr2(&config.ipv4_netmask),
					ip4_addr3(&config.ipv4_netmask),
					ip4_addr4(&config.ipv4_netmask)
				);

				xprompt_printf(
					cs->prompt_attr,
					"  rxpackets=%-10d txpackets=%d",
					status.rx_packet_count,
					status.tx_packet_count
				);
				xprompt_printf(cs->prompt_attr, "");
			}

		}
		else
		{
			break;
		}
	}

	return (TRUE);
}

/* --------------------------------------------------------
 * netif reset [id_mask]
 * ----------------------------------------------------- */

static bool_t xcommand_netif_reset(xcommand_callstack_t *cs)
{
	bool_t			success = FALSE;
	uint32_t		id_mask = 0xFFFFFFFF;
	const char *	argv_text;

	/* dst_gate_mask */
	if (xcommand_arg_pop_text(cs, &argv_text))
	{
		id_mask = xcommand_text2uint(argv_text);
	}

	if (id_mask != 0)
	{
		uint8_t id = 0;

		while (id_mask > 0)
		{
			if ((id_mask & 0x1) != 0)
			{
				xnetif_status_reset(id);
			}

			id_mask >>= 1;
			id++;
		}

		success = TRUE;
	}

	return (success);
}

/* =======================================================================
 * 		Command Map (netif)
 * ==================================================================== */

const struct xcommand_info XCMDLIST_NETIF[] =
{
		{ "start",  xcommand_netif_start,  XCOMMAND_NETIF_START_HELP },
		{ "stop",   xcommand_netif_stop,   "Network interface stop." },
		{ "list",   xcommand_netif_list,   "Network interface list show." },
		{ "reset",  xcommand_netif_reset,  "Network interface status reset." },
};

bool_t xcommand_netif(xcommand_callstack_t *cs)
{
	return (xcommand_sub_call(cs, XCMDLIST_NETIF, XCOUNTOF(XCMDLIST_NETIF)));
}

