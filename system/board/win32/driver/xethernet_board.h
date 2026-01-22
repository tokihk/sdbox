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

#include <pcap.h>


#define ADAPTER_NAME_LEN       128
#define ADAPTER_DESC_LEN       128


typedef pcap_t *	(*pcap_open_live_t)(const char *, int, int, int, char *);
typedef void		(*pcap_close_t)(pcap_t *);
typedef int			(*pcap_next_ex_t)(pcap_t *, struct pcap_pkthdr **, const u_char **);
typedef int			(*pcap_findalldevs_t)(pcap_if_t **, char *);
typedef void		(*pcap_freealldevs_t)(pcap_if_t *);

static pcap_open_live_t		m_pcap_open_live;
static pcap_close_t			m_pcap_close;
static pcap_next_ex_t		m_pcap_next_ex;
static pcap_findalldevs_t	m_pcap_findalldevs;
static pcap_freealldevs_t	m_pcap_freealldevs;


static struct
{
	HMODULE 					mod_wpcap;

	struct eth_addr				mac_addr;

	pcap_t *					adapter;
	char						name[ADAPTER_NAME_LEN + 1];
	char						description[ADAPTER_DESC_LEN + 1];

	uint8_t						ether_packet_work[1600];

} g_xether_win32;


static void xether_pcap_recv_packet_to_raw(const uint8_t *dev_packet, size_t dev_packet_len, uint8_t *raw_packet, size_t *raw_packet_size)
{
	size_t packet_size = 0;
	uint16_t ether_type = 0;

	if (dev_packet_len > 1500)
	{
		dev_packet_len = 1500;
	}

	/* Destination MAC address (6bytes) (全て自分宛にする) */
	memcpy(raw_packet, &g_xether_win32.mac_addr, sizeof(g_xether_win32.mac_addr));
	packet_size += 6;

	/* Source MAC address (6bytes) (全て自分からにする) */
	memcpy(raw_packet, &g_xether_win32.mac_addr, sizeof(g_xether_win32.mac_addr));
	packet_size += 6;

	/* EtherType (2bytes) */
	switch (dev_packet[0])
	{
		/* IPv4 */
		case 0x02:	ether_type = 0x0800;				break;

		/* IPv6 */
		case 0x18:	ether_type = 0x86DD;				break;

		/* Default */
		default:	ether_type = dev_packet_len - 4;	break;
	}

	raw_packet[packet_size++] = (ether_type >> 8) & 0xFF;
	raw_packet[packet_size++] = (ether_type >> 0) & 0xFF;

	/* Payload (dev_packet_len - 4) */
	memcpy(raw_packet + packet_size, dev_packet + 4, dev_packet_len - 4);
	packet_size += dev_packet_len - 4;

	*raw_packet_size = packet_size;
}

static struct pbuf *xether_pcap_packet_build(const uint8_t *packet, size_t packet_len)
{
	struct pbuf *pkt_top;
	struct pbuf *pkt_temp;
	size_t offset = 0;
	size_t length = packet_len;

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

	g_xether_win32.mod_wpcap = LoadLibrary(TEXT("wpcap.dll"));
	if (g_xether_win32.mod_wpcap != NULL)
	{
		m_pcap_open_live = (pcap_open_live_t)GetProcAddress(g_xether_win32.mod_wpcap, TEXT("pcap_open_live"));
		m_pcap_close = (pcap_close_t)GetProcAddress(g_xether_win32.mod_wpcap, TEXT("pcap_close"));
		m_pcap_next_ex = (pcap_next_ex_t)GetProcAddress(g_xether_win32.mod_wpcap, TEXT("pcap_next_ex"));
		m_pcap_findalldevs = (pcap_findalldevs_t)GetProcAddress(g_xether_win32.mod_wpcap, TEXT("pcap_findalldevs"));
		m_pcap_freealldevs = (pcap_freealldevs_t)GetProcAddress(g_xether_win32.mod_wpcap, TEXT("pcap_freealldevs"));
	}

}

static void xether_pcap_close(pcap_t *pcap)
{
	if (pcap != NULL)
	{
		m_pcap_close(pcap);
	}
}

static pcap_t *xether_pcap_open(uint16_t open_adapter_index)
{
	pcap_t *		pcap = NULL;
	char			errbuf[PCAP_ERRBUF_SIZE + 1];
	pcap_if_t *		alldevs;
	pcap_if_t *		adapter;
	uint16_t		adapter_count;

#if 0
	/* Retrieve the interfaces list */
	if (m_pcap_findalldevs(&alldevs, errbuf) == 0)
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
			pcap = m_pcap_open_live(
					adapter->name,				/* name of the device */
					65536,						/* portion of the packet to capture */
												/* 65536 guarantees that the whole packet will be captured on all the link layers */
					PCAP_OPENFLAG_PROMISCUOUS,	/* promiscuous mode */
					1,							/* wait 1 ms in ethernetif_poll */
					errbuf);					/* error buffer */
		}

		pcap_freealldevs(alldevs);
	}
#else
	pcap = m_pcap_open_live(
			"\\Device\\NPF_Loopback",				/* name of the device */
			65536,						/* portion of the packet to capture */
										/* 65536 guarantees that the whole packet will be captured on all the link layers */
			PCAP_OPENFLAG_PROMISCUOUS,	/* promiscuous mode */
			1,							/* wait 1 ms in ethernetif_poll */
			errbuf);					/* error buffer */
#endif

	return (pcap);
}

static bool_t xether_win32_open(const xether_config_t *config)
{
	bool_t is_opened = FALSE;

	g_xether_win32.mac_addr = config->mac_addr;

	/* イーサネットデバイスオープン */
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

	ret = m_pcap_next_ex(g_xether_win32.adapter, &pkt_header, &pkt_data);
	if (ret > 0)
	{
		size_t packet_size;

		/* ループバックデバイスから受信したPcapパケットをイーサパケットに変換 */
		xether_pcap_recv_packet_to_raw(pkt_data, pkt_header->caplen, g_xether_win32.ether_packet_work, &packet_size);

		pkt = xether_pcap_packet_build(g_xether_win32.ether_packet_work, packet_size);
	}

	return (pkt);
}

static bool_t xether_win32_send_packet_set(struct pbuf *p)
{
	return (TRUE);
}

static bool_t xether_win32_link_status_update(void)
{
	return (TRUE);
}

static inline void xether_init_board(void)
{
	xether_pcap_init();
}

static inline void xether_deinit_board(void)
{
}

XETHERNET_DEVICE_LIST_BEGIN()
  XETHERNET_DEVICE_LIST_ITEM(xether_win32),
XETHERNET_DEVICE_LIST_END()

#endif /* XETHER_BOARD_H_ */
