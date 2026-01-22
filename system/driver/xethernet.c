/*
 * xethernet.c
 *
 *  Created on: 2024/03/27
 *      Author: Hitoshi Kono
 */

#include "xethernet.h"


/* ----------------------------------------------
 * Macros for defining device lists
 * ------------------------------------------- */
struct xether_device_info
{
	bool_t						(*api_open)(const xether_config_t *config);
	void						(*api_close)(void);
	struct pbuf *				(*api_recv_packet_get)(void);
	bool_t						(*api_send_packet_set)(struct pbuf *p);
	bool_t						(*api_link_status_update)(void);
};

#define XETHERNET_DEVICE_LIST_BEGIN()		static const struct xether_device_info XETHERNET_DEVICE_LIST[] = {
#define XETHERNET_DEVICE_LIST_END()			};
#define XETHERNET_DEVICE_LIST_ITEM(prefix)	\
{															\
	.api_open=prefix##_open,								\
	.api_close=prefix##_close,								\
	.api_recv_packet_get=prefix##_recv_packet_get, 			\
	.api_send_packet_set=prefix##_send_packet_set,			\
	.api_link_status_update=prefix##_link_status_update,	\
}

/* ----------------------------------------------
 * Import board module
 * ------------------------------------------- */
#if XBOARD_ETHERNET_ENABLE
  #include "driver/xethernet_board.h"

  #define XBOARD_ETHERNET_NUM	(XCOUNTOF(XETHERNET_DEVICE_LIST))

#else
  #define XBOARD_ETHERNET_NUM				(0)

#endif


typedef struct xether_object
{
	uint16_t					dev_id;

	xether_config_t				config;

	uint32_t					tick_link_last;

	uint32_t					rx_packet_count;
	uint32_t					tx_packet_count;

	bool_t						is_opened;
	bool_t						is_linkup;
} xether_object_t;

static struct
{
	xether_object_t				objs[XBOARD_ETHERNET_NUM];

	bool_t						is_init;
} g_xether;


void xether_init(void)
{
#if XBOARD_ETHERNET_ENABLE
	xether_init_board();
#endif

	g_xether.is_init = TRUE;
}

void xether_deinit(void)
{
#if XBOARD_ETHERNET_ENABLE
	xether_deinit_board();
#endif

	g_xether.is_init = FALSE;
}

uint16_t xether_device_num_get(void)
{
	return (XBOARD_ETHERNET_NUM);
}

bool_t xether_open(uint16_t id, const xether_config_t *config)
{
	bool_t open_ok = FALSE;

#if XBOARD_ETHERNET_ENABLE

	if (   (g_xether.is_init)
		&& (id < XCOUNTOF(g_xether.objs))
		&& (config != NULL)
	) {
		xether_object_t *obj = &g_xether.objs[id];

		/* ステータス初期化 */
		obj->tx_packet_count = 0;
		obj->rx_packet_count = 0;

		if (!obj->is_opened)
		{
			obj->dev_id = id;
			obj->config = *config;

			/* デバイスオープン */
			if ((XETHERNET_DEVICE_LIST[id].api_open)(config))
			{
				obj->is_opened = TRUE;

				/* リンク状態更新 */
				xether_link_status_update(obj->dev_id);
			}
		}

		open_ok = obj->is_opened;
	}
#endif

	return (open_ok);
}

void xether_close(uint16_t id)
{
#if XBOARD_ETHERNET_ENABLE
	if (id < XCOUNTOF(g_xether.objs))
	{
		xether_object_t *obj = &g_xether.objs[id];

		/* デバイスクローズ */
		if (obj->is_opened)
		{
			obj->is_opened = FALSE;

			(XETHERNET_DEVICE_LIST[id].api_close)();
		}

		/* リンク状態更新 */
		xether_link_status_update(obj->dev_id);
	}
#endif
}

bool_t xether_config_get(uint16_t id, xether_config_t *config)
{
	bool_t get_ok = FALSE;

#if XBOARD_ETHERNET_ENABLE
	if (   (id < XCOUNTOF(g_xether.objs))
		&& (config != NULL)
	) {
		xether_object_t *obj = &g_xether.objs[id];

		*config = obj->config;

		get_ok = TRUE;
	}
#endif

	return (get_ok);
}

bool_t xether_link_status_update(uint16_t id)
{
	bool_t is_link = FALSE;

#if XBOARD_ETHERNET_ENABLE
	if (id < XCOUNTOF(g_xether.objs))
	{
		xether_object_t *obj = &g_xether.objs[id];

		if (obj->is_opened)
		{
			obj->is_linkup = (XETHERNET_DEVICE_LIST[id].api_link_status_update)();
		}
		else
		{
			obj->is_linkup = FALSE;
		}

		is_link = obj->is_linkup;
	}
#endif

	return (is_link);
}

struct pbuf *xether_recv_packet_get(uint16_t id)
{
	struct pbuf *pkt = NULL;

#if XBOARD_ETHERNET_ENABLE
	if (id < XCOUNTOF(g_xether.objs))
	{
		pkt = (XETHERNET_DEVICE_LIST[id].api_recv_packet_get)();
	}
#endif

	return (pkt);
}

bool_t xether_send_packet_set(uint16_t id, struct pbuf *pkt)
{
	bool_t set_ok = FALSE;

#if XBOARD_ETHERNET_ENABLE
	if (id < XCOUNTOF(g_xether.objs))
	{
		set_ok = (XETHERNET_DEVICE_LIST[id].api_send_packet_set)(pkt);
	}
#endif

	return (set_ok);
}
