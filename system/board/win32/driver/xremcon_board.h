/*
 * xremcon_board.h
 *
 *  Created on: 2025/01/03
 *      Author: Hitoshi Kono
 */

#ifndef XREMCON_BOARD_H_
#define XREMCON_BOARD_H_

#include <winsock2.h>


#define XREMCON_SYSLOG_UDP_PORT					(52000)

#define XREMCON_TERMINAL_TELNET_PORT			(52000)

#define XREMCON_GATE_MONITOR_UDP_PORT_BASE		(53000)


static struct
{
	WSADATA						wsa_data;

	SOCKET						terminal_local_sock;
	SOCKET						terminal_remote_sock;
	struct sockaddr_in			terminal_remote_addr;

	SOCKET						monitor_local_sock;
	struct sockaddr_in			monitor_remote_addr;
} g_xremcon_board;


static void xremcon_terminal_init(void)
{
	struct sockaddr_in	local_addr;
	BOOL				local_sockopt_reuse = TRUE;
	INT					sockaddr_size = sizeof(g_xremcon_board.terminal_remote_addr);
	struct timeval		timeout_recv;
	u_long				value;

	g_xremcon_board.terminal_local_sock = socket(AF_INET, SOCK_STREAM, 0);

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(XREMCON_TERMINAL_TELNET_PORT);
	local_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");;

	setsockopt(
		g_xremcon_board.terminal_local_sock,
		SOL_SOCKET,
		SO_REUSEADDR,
		(const char *)&local_sockopt_reuse,
		sizeof(local_sockopt_reuse)
	);

	timeout_recv.tv_sec = 0;
	timeout_recv.tv_usec = 0;
	setsockopt(
		g_xremcon_board.terminal_local_sock,
		SOL_SOCKET,
		SO_RCVTIMEO,
		(char *)&timeout_recv,
		sizeof(timeout_recv)
	);

	bind(g_xremcon_board.terminal_local_sock, (struct sockaddr *)&local_addr, sizeof(local_addr));

	listen(g_xremcon_board.terminal_local_sock, 1);

	g_xremcon_board.terminal_remote_sock = accept(
		g_xremcon_board.terminal_local_sock,
		(struct sockaddr *)&g_xremcon_board.terminal_remote_addr,
		&sockaddr_size
	);

	/* ノンブロッキングモード */
	value = 1;
	ioctlsocket(g_xremcon_board.terminal_remote_sock, FIONBIO, &value);
}

static void xremcon_monitor_init(void)
{
	g_xremcon_board.monitor_local_sock = socket(AF_INET, SOCK_DGRAM, 0);

	g_xremcon_board.monitor_remote_addr.sin_family = AF_INET;
	g_xremcon_board.monitor_remote_addr.sin_port = htons(XREMCON_GATE_MONITOR_UDP_PORT_BASE);
	g_xremcon_board.monitor_remote_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
}

static void xremcon_init_board(void)
{
	WSAStartup(MAKEWORD(2,0), &g_xremcon_board.wsa_data);

	xremcon_terminal_init();
	xremcon_monitor_init();
}

static void xremcon_deinit_board(void)
{
}

static size_t xremcon_terminal_read_board(uint8_t *buffer, size_t buffer_size)
{
	size_t read_size = 0;
	int result;

	result = recv(g_xremcon_board.terminal_remote_sock, (char *)buffer, buffer_size, 0);
	if (result > 0)
	{
		read_size = (size_t)result;
	}

	return (read_size);
}

static size_t xremcon_terminal_write_board(const uint8_t *data, size_t data_size)
{
	size_t write_size = 0;
	int result;

	result = send(g_xremcon_board.terminal_remote_sock, (const char *)data, data_size, 0);
	if (result > 0)
	{
		write_size = (size_t)result;
	}

	return (write_size);
}

static size_t xremcon_gate_monitor_write_board(uint8_t ch, const uint8_t *data, size_t data_size)
{
	size_t write_size = 0;
	int result;

	g_xremcon_board.monitor_remote_addr.sin_port = htons(XREMCON_GATE_MONITOR_UDP_PORT_BASE + ch);

	result = sendto(
			g_xremcon_board.monitor_local_sock,
			(const char *)data,
			data_size,
			0,
			(struct sockaddr *)&g_xremcon_board.monitor_remote_addr,
			sizeof(g_xremcon_board.monitor_remote_addr));

	if (result > 0)
	{
		write_size = (size_t)result;
	}

	return (write_size);
}


#endif /* XREMCON_BOARD_H_ */
