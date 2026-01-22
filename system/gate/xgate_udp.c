/*
 * xgate_udp.c
 *
 *  Created on: 2024/05/01
 *      Author: Hitoshi Kono
 */

#include "xgate_udp.h"

#include "prompt/xprompt.h"
#include "prompt/xcommand.h"

#include "core/xmalloc.h"

#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"

#include <stdio.h>


#undef  XGATE_CLASS_MOD_NAME
#define XGATE_CLASS_MOD_NAME	udp


XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME)
{
	ip_addr_t					local_addr;
	uint16_t					local_port;

	ip_addr_t					remote_addr;
	uint16_t					remote_port;
};

XGATE_CLASS_OBJECT(XGATE_CLASS_MOD_NAME)
{
	struct udp_pcb *			pcb;
};


const char XGATE_UDP_NAME[] = "udp";
const char XGATE_UDP_HELP[] = \
		"UDP Client.\n" \
		"  local_addr : Bind IPv4 Address.\n"
		"  local_port : Bind Port No.\n"
		"  remote_addr: Destination IPv4 Address.\n"
		"  remote_port: Destination Port No.\n"
		;


static void xgate_udp_lwip_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	xgate_object_recv((xgate_object_t *)arg, p->payload, (size_t)p->len);

	pbuf_free(p);
}

static void xgate_udp_config_init(xgate_class_config_t *gconf)
{
	XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc = (XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *)gconf->custom;

	ip_addr_set_any(FALSE, &gconfc->local_addr);
	gconfc->local_port = 50000;

	IP_ADDR4(&gconfc->remote_addr, 192, 168, 100, 200);
	gconfc->remote_port = 50100;
}

static void xgate_udp_config_deinit(xgate_class_config_t *gconf)
{
}

static bool_t xgate_udp_config_setup(xgate_class_config_t *gconf, const xcommand_arg_t *arg)
{
	XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc = (XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *)gconf->custom;

	bool_t update = FALSE;

	/* local_addr */
	if (strcmp(arg->name, "local_addr") == 0)
	{
		if (ipaddr_aton(arg->value, &gconfc->local_addr))
		{
			update = TRUE;
		}
	}

	/* local_port */
	else if (strcmp(arg->name, "local_port") == 0)
	{
		gconfc->local_port = (uint16_t)atoi(arg->value);
		update = TRUE;
	}

	/* remote_addr */
	else if (strcmp(arg->name, "remote_addr") == 0)
	{
		if (ipaddr_aton(arg->value, &gconfc->remote_addr))
		{
			update = TRUE;
		}
	}

	/* remote_port */
	else if (strcmp(arg->name, "remote_port") == 0)
	{
		gconfc->remote_port = (uint16_t)atoi(arg->value);
		update = TRUE;
	}

	return (update);
}

static void xgate_udp_config_print(xgate_class_config_t *gconf, enum xprompt_attr attr)
{
	XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *gconfc = (XGATE_CLASS_CONFIG(XGATE_CLASS_MOD_NAME) *)gconf->custom;

	char	local_addr[IP4ADDR_STRLEN_MAX + 1] = {0};
	char	remote_addr[IP4ADDR_STRLEN_MAX + 1] = {0};

	if (ip_addr_get_ip4_u32(&gconfc->local_addr) == 0)
	{
		snprintf(local_addr, XCOUNTOF(local_addr), "ANY_ADDR");
	}
	else
	{
		ipaddr_ntoa_r(&gconfc->local_addr, local_addr, XCOUNTOF(local_addr));
	}

	ipaddr_ntoa_r(&gconfc->remote_addr, remote_addr, XCOUNTOF(remote_addr));

	/* Config Text */
	xprompt_printf(
		attr,
		"local %s:%d  remote %s:%d",
		local_addr, gconfc->local_port,
		remote_addr, gconfc->remote_port
	);
}

static bool_t xgate_udp_open(xgate_object_t *gobj)
{
	XGATE_CLASS_OBJECT_VAR(gobj, XGATE_CLASS_MOD_NAME, gobjc);
	XGATE_CLASS_CONFIG_VAR(gobj, XGATE_CLASS_MOD_NAME, gconfc);

	bool_t success = FALSE;

	/* PCB生成 */
	gobjc->pcb = udp_new();
	if (gobjc->pcb != NULL)
	{
		/* バインド */
		if (udp_bind(gobjc->pcb, &(gconfc->local_addr), gconfc->local_port) == ERR_OK)
		{
			success = TRUE;
		}
	}

	if (success)
	{
		/* 受信開始 */
		udp_recv(gobjc->pcb, xgate_udp_lwip_recv, gobj);

	}
	else
	{
		/* エラー処理 */
		if (gobjc->pcb != NULL)
		{
			udp_remove(gobjc->pcb);
			gobjc->pcb = NULL;
		}
	}

	return (success);
}

static bool_t xgate_udp_close(xgate_object_t *gobj)
{
	XGATE_CLASS_OBJECT_VAR(gobj, XGATE_CLASS_MOD_NAME, gobjc);

	if (gobjc != NULL)
	{
		/* PCB削除 */
		if (gobjc->pcb != NULL)
		{
			udp_remove(gobjc->pcb);
			gobjc->pcb = NULL;
		}
	}

	return (TRUE);
}

static void xgate_udp_poll(xgate_object_t *gobj)
{
}

static void xgate_udp_send(xgate_object_t *gobj, const uint8_t *data, size_t data_size)
{
	XGATE_CLASS_OBJECT_VAR(gobj, XGATE_CLASS_MOD_NAME, gobjc);
	XGATE_CLASS_CONFIG_VAR(gobj, XGATE_CLASS_MOD_NAME, gconfc);

	struct pbuf *	tx_buf;

	tx_buf = pbuf_alloc(PBUF_TRANSPORT, data_size, PBUF_RAM);
	if (tx_buf != NULL)
	{
		pbuf_take(tx_buf, data, data_size);

		udp_sendto(gobjc->pcb, tx_buf, &gconfc->remote_addr, gconfc->remote_port);

		pbuf_free(tx_buf);
	}
}

XGATE_CLASS_DEFINE(XGATE_CLASS_MOD_NAME, XGATE_UDP_NAME, XGATE_UDP_HELP)

