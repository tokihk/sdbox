#ifndef LWIP_PCAPIF_H
#define LWIP_PCAPIF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/err.h"

/** Set to 1 to let rx use an own thread (only for NO_SYS==0).
 * If set to 0, ethernetif_poll is used to poll for packets.
 */
#ifndef PCAPIF_RX_USE_THREAD
#define PCAPIF_RX_USE_THREAD !NO_SYS
#endif
#if PCAPIF_RX_USE_THREAD && NO_SYS
#error "Can't create a dedicated RX thread with NO_SYS==1"
#endif

#define ADAPTER_NAME_LEN       128
#define ADAPTER_DESC_LEN       128


struct netif;

/* Packet Adapter information */
struct pcapif_private {
  void            *input_fn_arg;
  pcap_t          *adapter;
  char             name[ADAPTER_NAME_LEN];
  char             description[ADAPTER_DESC_LEN];
  int              shutdown_called;
#if PCAPIF_RX_USE_THREAD
  volatile int     rx_run;
  volatile int     rx_running;
#endif /* PCAPIF_RX_USE_THREAD */
#if PCAPIF_HANDLE_LINKSTATE
  struct pcapifh_linkstate *link_state;
  enum pcapifh_link_event last_link_event;
#endif /* PCAPIF_HANDLE_LINKSTATE */
#if PCAPIF_RECEIVE_PROMISCUOUS
  struct pcapipf_pending_packet packets[PCAPIF_LOOPBACKFILTER_NUM_TX_PACKETS];
  struct pcapipf_pending_packet *tx_packets;
  struct pcapipf_pending_packet *free_packets;
#endif /* PCAPIF_RECEIVE_PROMISCUOUS */
};

err_t pcapif_init    (struct pcapif_private *pa);
void  pcapif_shutdown(struct pcapif_private *pa);
#if !PCAPIF_RX_USE_THREAD
void  pcapif_poll    (struct pcapif_private *pa);
#endif /* !PCAPIF_RX_USE_THREAD */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_PCAPIF_H */
