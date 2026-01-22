/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : Target/lwipopts.h
  * Description        : This file overrides LwIP stack default configuration
  *                      done in opt.h file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef __LWIPOPTS__H__
#define __LWIPOPTS__H__

#include "lwipopts_board.h"

//#include "config/xconfig_task.h"

#ifdef __cplusplus
 extern "C" {
#endif


//#define LWIP_DEBUG
#define TCP_OUTPUT_DEBUG			LWIP_DBG_ON
#define UDP_DEBUG					LWIP_DBG_ON

/*
 * NO_SYS==1: Use lwIP without OS-awareness (no thread, semaphores, mutexes or mboxes).
 * This means threaded APIs cannot be used (socket, netconn, i.e. everything in the 'api' folder),
 * only the callback-style raw API is available
 * (and you have to watch out for yourself that you don't access lwIP functions/structures from more than one context at a time!)
 */
 #ifndef NO_SYS
 #define NO_SYS						1
 #endif

 #ifndef SYS_LIGHTWEIGHT_PROT
 #define SYS_LIGHTWEIGHT_PROT		!NO_SYS
 #endif

 #ifndef LWIP_NETCONN
 #define LWIP_NETCONN				0
 #endif

 #ifndef LWIP_SOCKET
 #define LWIP_SOCKET				0
 #endif

 #ifndef MEM_SIZE
 #define MEM_SIZE					(24 * 1024)
 #endif

 /* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
    sends a lot of data out of ROM (or other static memory), this
    should be set high. */
 #ifndef MEMP_NUM_PBUF
 #define MEMP_NUM_PBUF				16
 #endif
 /* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
    per active UDP "connection". */
 #ifndef MEMP_NUM_UDP_PCB
 #define MEMP_NUM_UDP_PCB			6
 #endif
 /* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
    connections. */
 #ifndef MEMP_NUM_TCP_PCB
 #define MEMP_NUM_TCP_PCB			10
 #endif
 /* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
    connections. */
 #ifndef MEMP_NUM_TCP_PCB_LISTEN
 #define MEMP_NUM_TCP_PCB_LISTEN	8
 #endif
 /* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
    segments. */
 #ifndef MEMP_NUM_TCP_SEG
 #define MEMP_NUM_TCP_SEG			22
 #endif
 /* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
    timeouts. */
 #ifndef MEMP_NUM_SYS_TIMEOUT
 #define MEMP_NUM_SYS_TIMEOUT		10
 #endif
 /* MEMP_NUM_REASS_DATA: The number of whole IP packets
    queued for reassembly. */
 #ifndef MEMP_NUM_REASSDATA
 #define MEMP_NUM_REASSDATA			4
 #endif

 /* ---------- Pbuf options ---------- */
 /* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
 #ifndef PBUF_POOL_SIZE
 #define PBUF_POOL_SIZE				5
 #endif

 /* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
 /* Default value is defined in lwip\src\include\lwip\opt.h as
  * LWIP_MEM_ALIGN_SIZE(TCP_MSS+40+PBUF_LINK_ENCAPSULATION_HLEN+PBUF_LINK_HLEN)*/

 /* ---------- TCP options ---------- */
 #ifndef LWIP_TCP
 #define LWIP_TCP					1
 #endif

 #ifndef TCP_TTL
 #define TCP_TTL					255
 #endif

 /* Controls if TCP should queue segments that arrive out of
    order. Define to 0 if your device is low on memory. */
 #ifndef TCP_QUEUE_OOSEQ
 #define TCP_QUEUE_OOSEQ			0
 #endif

 /* TCP Maximum segment size. */
 #ifndef TCP_MSS
 #define TCP_MSS					(1500 - 40) /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */
 #endif

 /* TCP sender buffer space (bytes). */
 #ifndef TCP_SND_BUF
 #define TCP_SND_BUF				(6 * TCP_MSS) // 2
 #endif

 /* TCP sender buffer space (pbufs). This must be at least = 2 *
    TCP_SND_BUF/TCP_MSS for things to work. */
 #ifndef TCP_SND_QUEUELEN
 #define TCP_SND_QUEUELEN			(3 * TCP_SND_BUF) / TCP_MSS // 6
 #endif

 /* TCP receive window. */
 #ifndef TCP_WND
 #define TCP_WND					(2 * TCP_MSS)
 #endif

 /* Enable backlog*/
 #ifndef TCP_LISTEN_BACKLOG
 #define TCP_LISTEN_BACKLOG			1
 #endif

 /* ---------- ICMP options ---------- */
 #ifndef LWIP_ICMP
 #define LWIP_ICMP					1
 #endif

 /* ---------- DHCP options ---------- */
 /* Enable DHCP module. */
 #ifndef LWIP_DHCP
 #define LWIP_DHCP					1
 #endif

 /* ---------- UDP options ---------- */
 #ifndef LWIP_UDP
 #define LWIP_UDP					1
 #endif
 #ifndef UDP_TTL
 #define UDP_TTL					255
 #endif

 /* ---------- Statistics options ---------- */
 #ifndef LWIP_STATS
 #define LWIP_STATS					0
 #endif

 /**
  * LWIP_NETIF_EXT_STATUS_CALLBACK==1: Support an extended callback function
  * for several netif related event that supports multiple subscribers.
  * @see netif_ext_status_callback
  */
 #define LWIP_NETIF_EXT_STATUS_CALLBACK		0

 /**
  * IP_REASS_MAX_PBUFS: Number of buffers reserved for IP fragment reassembly.
  */
 #ifndef IP_REASS_MAX_PBUFS
 #define IP_REASS_MAX_PBUFS			4
 #endif


#if 0
#define TCPIP_MBOX_SIZE				32
#define TCPIP_THREAD_STACKSIZE		1024
#define TCPIP_THREAD_PRIO			XTASKPRIO_LWIP_TCPIP_THREAD
#endif

#ifdef __cplusplus
}
#endif
#endif /*__LWIPOPTS__H__ */
