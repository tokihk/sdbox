/*
 * xremcon_board.h
 *
 *  Created on: 2025/01/03
 *      Author: Hitoshi Kono
 */

#ifndef XREMCON_BOARD_H_
#define XREMCON_BOARD_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>


#define XREMCON_TCP_PORT_TERMINAL					(51100)
#define XREMCON_TCP_PORT_VIRTUAL_DEVICE				(51101)

#define XREMCON_UDP_PORT_SYSLOG						(51100)
#define XREMCON_UDP_PORT_GATE_MONITOR_BASE			(52000)


static struct
{
	int							terminal_local_sock;
	int							terminal_remote_sock;
	struct sockaddr_in			terminal_remote_addr;

	int							virtdev_local_sock;
	int							virtdev_remote_sock;
	struct sockaddr_in			virtdev_remote_addr;

	int							monitor_local_sock;
	struct sockaddr_in			monitor_remote_addr;
} g_xremcon_board;


static void xremcon_terminal_init(void)
{
	struct sockaddr_in	local_addr;
	socklen_t			local_sockopt_reuse = 1;
	socklen_t			sockaddr_size = sizeof(g_xremcon_board.terminal_remote_addr);
	struct timeval		timeout_recv;
	u_long				value;
	int					retval;

	g_xremcon_board.terminal_local_sock = socket(AF_INET, SOCK_STREAM, 0);

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(XREMCON_TCP_PORT_TERMINAL);
	local_addr.sin_addr.s_addr = INADDR_ANY;

	setsockopt(
		g_xremcon_board.terminal_local_sock,
		SOL_SOCKET,
		SO_REUSEADDR,
		(const char *)&local_sockopt_reuse,
		sizeof(local_sockopt_reuse)
	);

	retval = bind(g_xremcon_board.terminal_local_sock, (struct sockaddr *)&local_addr, sizeof(local_addr));

	retval = listen(g_xremcon_board.terminal_local_sock, 1);

	g_xremcon_board.terminal_remote_sock = accept(
		g_xremcon_board.terminal_local_sock,
		(struct sockaddr *)&g_xremcon_board.terminal_remote_addr,
		&sockaddr_size
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

	/* ノンブロッキングモード */
	value = 1;
	ioctl(g_xremcon_board.terminal_remote_sock, FIONBIO, &value);
}

static void xremcon_virtual_device_init(void)
{
	struct sockaddr_in	local_addr;
	socklen_t			local_sockopt_reuse = 1;
	socklen_t			sockaddr_size = sizeof(g_xremcon_board.virtdev_remote_addr);
	struct timeval		timeout_recv;
	u_long				value;

	g_xremcon_board.virtdev_local_sock = socket(AF_INET, SOCK_STREAM, 0);

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(XREMCON_TCP_PORT_VIRTUAL_DEVICE);
	local_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	setsockopt(
		g_xremcon_board.virtdev_local_sock,
		SOL_SOCKET,
		SO_REUSEADDR,
		(const char *)&local_sockopt_reuse,
		sizeof(local_sockopt_reuse)
	);

	bind(g_xremcon_board.virtdev_local_sock, (struct sockaddr *)&local_addr, sizeof(local_addr));

	listen(g_xremcon_board.virtdev_local_sock, 1);

	g_xremcon_board.virtdev_remote_sock = accept(
		g_xremcon_board.virtdev_local_sock,
		(struct sockaddr *)&g_xremcon_board.virtdev_remote_addr,
		&sockaddr_size
	);

	timeout_recv.tv_sec = 0;
	timeout_recv.tv_usec = 0;
	setsockopt(
		g_xremcon_board.virtdev_local_sock,
		SOL_SOCKET,
		SO_RCVTIMEO,
		(char *)&timeout_recv,
		sizeof(timeout_recv)
	);


	/* ノンブロッキングモード */
	value = 1;
	ioctl(g_xremcon_board.virtdev_remote_sock, FIONBIO, &value);
}

static void xremcon_monitor_init(void)
{
	g_xremcon_board.monitor_local_sock = socket(AF_INET, SOCK_DGRAM, 0);

	g_xremcon_board.monitor_remote_addr.sin_family = AF_INET;
	g_xremcon_board.monitor_remote_addr.sin_port = htons(XREMCON_UDP_PORT_GATE_MONITOR_BASE);
	g_xremcon_board.monitor_remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

static void xremcon_init_board(void)
{
	xremcon_terminal_init();
	xremcon_virtual_device_init();
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

	g_xremcon_board.monitor_remote_addr.sin_port = htons(XREMCON_UDP_PORT_GATE_MONITOR_BASE + ch);

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

static inline void xremcon_signal_out_set_board(uint8_t ch, bool_t on)
{
}

static inline void xremcon_signal_out_toggle_board(uint8_t ch)
{
}


#endif /* XREMCON_BOARD_H_ */
