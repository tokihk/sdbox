/*
 * xethernet_board.h
 *
 *  Created on: 2024/03/27
 *      Author: Hitoshi Kono
 */

#ifndef XETHERNET_BOARD_H_
#define XETHERNET_BOARD_H_

#include "pcapif_helper.h"

#include "lwip/pbuf.h"

#include "pcap.h"


#define ADAPTER_NAME_LEN       128
#define ADAPTER_DESC_LEN       128


static struct
{
	pcap_t *					adapter;
	char						name[ADAPTER_NAME_LEN + 1];
	char						description[ADAPTER_DESC_LEN + 1];

} g_xether_win32;


static struct pbuf *xether_pcap_packet_build(const void *packet, int packet_len)
{
	struct pbuf *pkt_top;
	struct pbuf *pkt_temp;
	int offset = 0;
	int length = packet_len;

	/* We allocate a pbuf chain of pbufs from the pool. */
	pkt_top = pbuf_alloc(PBUF_RAW, (u16_t)length + ETH_PAD_SIZE, PBUF_POOL);
	if (pkt_top != NULL)
	{
		/* We iterate over the pbuf chain until we have read the entire
		packet into the pbuf. */
		for (pkt_temp = pkt_top; pkt_temp != NULL; pkt_temp = pkt_temp->next)
		{
			u16_t copy_len = pkt_temp->len;
			/* Read enough bytes to fill this pbuf in the chain. The
			 available data in the pbuf is given by the q->len
			 variable. */
			/* read data into(q->payload, q->len); */
			if (pkt_temp == pkt_top)
			{
				#if ETH_PAD_SIZE
				LWIP_ASSERT("q->len >= ETH_PAD_SIZE", q->len >= ETH_PAD_SIZE);
				copy_len -= ETH_PAD_SIZE;
				#endif /* ETH_PAD_SIZE*/
				MEMCPY(&((char*)pkt_temp->payload)[ETH_PAD_SIZE], &((const char*)packet)[offset], copy_len);
			}
			else
			{
				MEMCPY(pkt_temp->payload, &((const char*)packet)[offset], copy_len);
			}
			offset += copy_len;
			length -= copy_len;
			if (length <= 0)
			{
				break;
			}
		}
	}

	return (pkt_top);
}

static void xether_pcap_init(void)
{
	char npcap_dir[512];
	unsigned int len;

	len = GetSystemDirectory(npcap_dir, 480);
	if (len > 0) {
		strcat_s(npcap_dir, 512, "\\Npcap");

		SetDllDirectory(npcap_dir);
	}
}

static void xether_pcap_close(pcap_t *pcap)
{
	if (pcap != NULL)
	{
		pcap_close(pcap);
	}
}

static pcap_t *xether_pcap_open(uint16_t open_adapter_index)
{
	pcap_t *		pcap = NULL;
	char			errbuf[PCAP_ERRBUF_SIZE + 1];
	pcap_if_t *		alldevs;
	pcap_if_t *		adapter;
	uint16_t		adapter_count;

	/* Retrieve the interfaces list */
	if (pcap_findalldevs(&alldevs, errbuf) == 0)
	{
		/* get number of adapters and adapter pointer */
		for (adapter = alldevs, adapter_count = 0; adapter != NULL; adapter = adapter->next, adapter_count++)
		{
			if (adapter_count == open_adapter_index)
			{
				break;
			}
		}

		if (adapter != NULL)
		{
			/* Open the device */
			pcap = pcap_open_live(
					adapter->name,				/* name of the device */
					65536,						/* portion of the packet to capture */
												/* 65536 guarantees that the whole packet will be captured on all the link layers */
					PCAP_OPENFLAG_PROMISCUOUS,	/* promiscuous mode */
					1,							/* wait 1 ms in ethernetif_poll */
					errbuf);					/* error buffer */
		}

		pcap_freealldevs(alldevs);
	}

	return (pcap);
}

static bool_t xether_win32_open(const xether_config_t *config)
{
	bool_t is_opened = FALSE;

	/* Do whatever else is needed to initialize interface. */
	g_xether_win32.adapter = xether_pcap_open(0);
	if (g_xether_win32.adapter != NULL)
	{
		is_opened = TRUE;
	}

	return (is_opened);
}

static void xether_win32_close(void)
{
	if (g_xether_win32.adapter != NULL)
	{
		xether_pcap_close(g_xether_win32.adapter);
		g_xether_win32.adapter = NULL;
	}
}

static struct pbuf *xether_win32_recv_packet_get(void)
{
	struct pbuf *			pkt = NULL;
	struct pcap_pkthdr *	pkt_header;
	const u_char *			pkt_data;
	int						ret;

	ret = pcap_next_ex(g_xether_win32.adapter, &pkt_header, &pkt_data);
	if (ret > 0)
	{
		pkt = xether_pcap_packet_build(pkt_data, pkt_header->caplen);
	}

	return (pkt);
}

static bool_t xether_win32_send_packet_set(struct pbuf *p)
{
	return (TRUE);
}

static bool_t xether_win32_link_status_update(void)
{
	bool_t is_link = FALSE;

	return (is_link);
}

static inline void xether_init_board(const struct xether_device_info *dev_list, uint16_t dev_list_count)
{
	xether_pcap_init();
}

static inline void xether_shutdown_board(const struct xether_device_info *dev_list, uint16_t dev_list_count)
{
}

XETHERNET_DEVICE_LIST_BEGIN()
  XETHERNET_DEVICE_LIST_ITEM(xether_win32),
XETHERNET_DEVICE_LIST_END()

#endif /* XETHER_BOARD_H_ */
