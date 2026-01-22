// system_sdsgw.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include <winsock2.h>
#include <windows.h>

#include "pcap.h"

#if 0
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
#if 1
	char npcap_dir[512];
	unsigned int len;

	len = GetSystemDirectory(npcap_dir, 480);
	if (len > 0) {
		strcat_s(npcap_dir, 512, "\\Npcap");

		SetDllDirectory(npcap_dir);
	}

	pcap_t* pcap = NULL;
	char			errbuf[PCAP_ERRBUF_SIZE + 1];
	pcap_if_t* alldevs;
	pcap_if_t* adapter;
	uint16_t		adapter_count;

	/* Retrieve the interfaces list */
	if (pcap_findalldevs(&alldevs, errbuf) == 0)
	{
		/* get number of adapters and adapter pointer */
		for (adapter = alldevs, adapter_count = 0; adapter != NULL; adapter = adapter->next, adapter_count++)
		{
			if (adapter_count == 0xFFFF)
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
#endif

    return (0);
}


