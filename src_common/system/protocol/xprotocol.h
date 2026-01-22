/*
 * xprotocol.h
 *
 *  Created on: 2024/05/01
 *      Author: Hitoshi Kono
 */

#ifndef XPROTOCOL_H_
#define XPROTOCOL_H_

#include "core/xstddef.h"


#define XPROTOCOL_CLASS_NAME_LEN_MAX				(24)

#define XPROTOCOL_CLASS_DECLARATION(mname)			XPROTOCOL_CLASS_DECLARATION_(mname)
#define XPROTOCOL_CLASS_DECLARATION_(mname)			void xprotocol_##mname##_class(xprotocol_class_t *gclass)

#define XPROTOCOL_CLASS_CONFIG(mname)				XPROTOCOL_CLASS_CONFIG_(mname)
#define XPROTOCOL_CLASS_CONFIG_(mname)				struct xprotocol_##mname##_config

#define XPROTOCOL_CLASS_OBJECT(mname)				XPROTOCOL_CLASS_OBJECT_(mname)
#define XPROTOCOL_CLASS_OBJECT_(mname)				struct xprotocol_##mname##_object

#define XPROTOCOL_CLASS_DEFINE(mname,dname,dhelp)	XPROTOCOL_CLASS_DEFINE_(mname, dname, dhelp)
#define XPROTOCOL_CLASS_DEFINE_(mname,dname,dhelp)							\
void xprotocol_##mname##_class(xprotocol_class_t *class_info)				\
{																			\
	class_info->name = dname;												\
	class_info->help = dhelp;												\
	class_info->config_size = sizeof(XPROTOCOL_CLASS_CONFIG(mname));		\
	class_info->object_size = sizeof(XPROTOCOL_CLASS_OBJECT(mname));		\
	class_info->api.config_init = xprotocol_##mname##_config_init;			\
	class_info->api.config_deinit = xprotocol_##mname##_config_deinit;		\
	class_info->api.config_setup = xprotocol_##mname##_config_setup;		\
	class_info->api.config_print = xprotocol_##mname##_config_print;		\
	class_info->api.open = xprotocol_##mname##_open;						\
	class_info->api.close = xprotocol_##mname##_close;						\
	class_info->api.recv = xprotocol_##mname##_recv;						\
	class_info->api.status_update = xprotocol_##mname##_status_update;		\
}																			\

#define XPROTOCOL_CLASS_CONFIG_PTR(pobj)				((pobj)->class_config.custom)
#define XPROTOCOL_CLASS_OBJECT_PTR(pobj)				((pobj)->class_obj.custom)

#define XPROTOCOL_CLASS_CONFIG_VAR(pobj,mname,vname)	XPROTOCOL_CLASS_CONFIG_VAR_(pobj,mname,vname)
#define XPROTOCOL_CLASS_CONFIG_VAR_(pobj,mname,vname)	XPROTOCOL_CLASS_CONFIG(mname) *vname = (XPROTOCOL_CLASS_CONFIG(mname) *)XPROTOCOL_CLASS_CONFIG_PTR(pobj)

#define XPROTOCOL_CLASS_OBJECT_VAR(pobj,mname,vname)	XPROTOCOL_CLASS_OBJECT_VAR_(pobj,mname,vname)
#define XPROTOCOL_CLASS_OBJECT_VAR_(pobj,mname,vname)	XPROTOCOL_CLASS_OBJECT(mname) *vname = (XPROTOCOL_CLASS_OBJECT(mname) *)XPROTOCOL_CLASS_OBJECT_PTR(pobj)


struct xprotocol_class;
struct xprotocol_class_config;

struct xprotocol_object;
struct xprotocol_config;
struct xprotocol_status;


typedef struct xprotocol_class			xprotocol_class_t;
typedef struct xprotocol_class_config	xprotocol_class_config_t;

typedef struct xprotocol_object			xprotocol_object_t;
typedef struct xprotocol_config			xprotocol_config_t;


typedef void					(*xprotocol_class_func_t)(xprotocol_class_t *gclass);

typedef void					(*xprotocol_config_init_func_t)(xprotocol_class_config_t *gclass_conf);
typedef void					(*xprotocol_config_deinit_func_t)(xprotocol_class_config_t *gclass_conf);
typedef bool_t					(*xprotocol_config_setup_func_t)(xprotocol_class_config_t *gclass_conf, const xcommand_arg_t *arg);
typedef void					(*xprotocol_config_print_func_t)(xprotocol_class_config_t *gclass_conf, enum xprompt_attr attr);

typedef bool_t					(*xprotocol_open_func_t)(xprotocol_object_t *gobj);
typedef bool_t					(*xprotocol_close_func_t)(xprotocol_object_t *gobj);

typedef void					(*xprotocol_send_func_t)(xprotocol_object_t *gobj, const uint8_t *data, size_t data_size);


typedef struct xprotocol_status
{
	enum xprotocol_open_status		open_status;

	enum xprotocol_status_mask		status_mask;

	uint32_t					send_packet_counter;
	uint32_t					recv_packet_counter;
} xprotocol_status_t;

typedef struct xprotocol_class_api
{
	xprotocol_config_init_func_t	config_init;
	xprotocol_config_deinit_func_t	config_deinit;
	xprotocol_config_setup_func_t	config_setup;
	xprotocol_config_print_func_t	config_print;

	xprotocol_open_func_t			open;
	xprotocol_close_func_t			close;

	xprotocol_send_func_t			send;
} xprotocol_class_api_t;

typedef struct xprotocol_class
{
	const char *				name;
	const char *				help;

	size_t						config_size;
	size_t						object_size;

	xprotocol_class_api_t			api;
} xprotocol_class_t;

typedef struct xprotocol_class_config
{
	xprotocol_class_t *				gclass;
	void *						custom;
} xprotocol_class_config_t;

typedef struct xprotocol_class_object
{
	xprotocol_class_t *				gclass;
	void *						custom;
} xprotocol_class_object_t;

typedef struct xprotocol_config
{
	char						name[XGATE_OBJECT_NAME_LEN_MAX + 1];

	bool_t						xflow_enable;
	uint8_t						xflow_code_xon;
	uint8_t						xflow_code_xoff;
} xprotocol_config_t;

typedef struct xprotocol_object
{
	xprotocol_class_object_t	class_obj;

	xprotocol_class_config_t	class_config;
	xprotocol_class_config_t	class_config_next;

	xprotocol_config_t			config;
	xprotocol_status_t			status;

	uint8_t *					send_buffer;
	size_t						send_buffer_size;


	bool_t						open_req;
	bool_t						reopen_req;

	uint32_t					forwarding_dst_gate_mask;		/* 自身が受信したパケットを転送するゲート一覧 */
	uint32_t					forwarding_src_gate_mask;		/* 自身にパケットを転送してくるゲート一覧     */

	uint32_t					dst_gate_rts_mask;				/* 転送先ゲートのRequest to Send状態一覧 */
} xprotocol_object_t;


void							xprotocol_init(void);

void							xprotocol_class_enumeration(void (*callback)(const xprotocol_class_t *gclass, void *arg), void *arg);
void							xprotocol_object_enumeration(void (*callback)(uint8_t gate_no, const xprotocol_object_t *gobj, void *arg), void *arg);

xprotocol_object_t *			xprotocol_object_new(const char *class_name, uint16_t paramc, char * const * paramv, xgate_object_t *gobj);
void							xprotocol_object_delete(xprotocol_object_t *pobj);

void							xprotocol_object_open(xprotocol_object_t *pobj);
void							xprotocol_object_close(xprotocol_object_t *pobj);

void							xprotocol_object_send(xprotocol_object_t *pobj, const uint8_t *data, size_t data_size);
void							xprotocol_object_recv(xprotocol_object_t *pobj, const uint8_t *data, size_t data_size);

void							xprotocol_object_status_set(xprotocol_object_t *pobj, );


#endif /* XPROTOCOL_H_ */
