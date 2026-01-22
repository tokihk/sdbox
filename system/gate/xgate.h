/*
 * xgate.h
 *
 *  Created on: 2024/05/01
 *      Author: Hitoshi Kono
 */

#ifndef XGATE_H_
#define XGATE_H_

#include "prompt/xcommand.h"
#include "prompt/xprompt.h"


#define XGATE_CLASS_NAME_LEN_MAX				(24)

#define XGATE_OBJECT_NUM_MAX					(10)
#define XGATE_OBJECT_NAME_LEN_MAX				(12)

#define XGATE_PACKET_COUNTER_MAX				(0xFFFFFFFFU)

#define XGATE_CLASS_DECLARATION(mname)			XGATE_CLASS_DECLARATION_(mname)
#define XGATE_CLASS_DECLARATION_(mname)			void xgate_##mname##_class(xgate_class_t *gclass)

#define XGATE_CLASS_CONFIG(mname)				XGATE_CLASS_CONFIG_(mname)
#define XGATE_CLASS_CONFIG_(mname)				struct xgate_##mname##_config

#define XGATE_CLASS_OBJECT(mname)				XGATE_CLASS_OBJECT_(mname)
#define XGATE_CLASS_OBJECT_(mname)				struct xgate_##mname##_object

#define XGATE_CLASS_DEFINE(mname, dname, dhelp)	XGATE_CLASS_DEFINE_(mname, dname, dhelp)
#define XGATE_CLASS_DEFINE_(mname, dname, dhelp)					\
void xgate_##mname##_class(xgate_class_t *class_info)					\
{																		\
	class_info->name = dname;											\
	class_info->help = dhelp;											\
	class_info->config_size = sizeof(XGATE_CLASS_CONFIG(mname));		\
	class_info->object_size = sizeof(XGATE_CLASS_OBJECT(mname));		\
	class_info->api.config_init = xgate_##mname##_config_init;			\
	class_info->api.config_deinit = xgate_##mname##_config_deinit;		\
	class_info->api.config_setup = xgate_##mname##_config_setup;		\
	class_info->api.config_print = xgate_##mname##_config_print;		\
	class_info->api.gate_open = xgate_##mname##_open;					\
	class_info->api.gate_close = xgate_##mname##_close;					\
	class_info->api.gate_poll = xgate_##mname##_poll;					\
	class_info->api.gate_send = xgate_##mname##_send;					\
}																		\

#define XGATE_OBJECT_IS_VALID(gobj)						((gobj)->class_config.gclass != NULL)

#define XGATE_CLASS_CONFIG_PTR(gobj)					((gobj)->class_config.custom)
#define XGATE_CLASS_OBJECT_PTR(gobj)					((gobj)->class_obj.custom)

#define XGATE_CLASS_CONFIG_VAR(gobj,mname,var_name)		XGATE_CLASS_CONFIG_VAR_(gobj,mname,var_name)
#define XGATE_CLASS_CONFIG_VAR_(gobj,mname,var_name)	XGATE_CLASS_CONFIG(mname) *var_name = (XGATE_CLASS_CONFIG(mname) *)XGATE_CLASS_CONFIG_PTR(gobj)

#define XGATE_CLASS_OBJECT_VAR(gobj,mname,var_name)		XGATE_CLASS_OBJECT_VAR_(gobj,mname,var_name)
#define XGATE_CLASS_OBJECT_VAR_(gobj,mname,var_name)	XGATE_CLASS_OBJECT(mname) *var_name = (XGATE_CLASS_OBJECT(mname) *)XGATE_CLASS_OBJECT_PTR(gobj)


enum xgate_open_status
{
	XGATE_CLOSED,
	XGATE_CLOSING_BUSY,
	XGATE_OPENING_BUSY,
	XGATE_OPENED,
};

enum xgate_print_contents_mask
{
	XGATE_PCMASK_CONFIG		= 1u << 0,
	XGATE_PCMASK_STATUS		= 1u << 1,

	XGATE_PCMASK_ALL		= 0xFFFFFFFFu,
};

enum xgate_status_mask
{
	XGATE_STMASK_REQUEST_TO_SEND	= 1u << 0,
};

enum xgate_monitor_contents
{
	XGATE_MONITOR_RECV_DATA,
	XGATE_MONITOR_SEND_DATA,

	XGATE_MONITOR_CONTENTS_MAX,
};


struct xgate_class;
struct xgate_class_config;

struct xgate_object;
struct xgate_config;
struct xgate_status;


typedef struct xgate_class			xgate_class_t;
typedef struct xgate_class_config	xgate_class_config_t;

typedef struct xgate_object			xgate_object_t;
typedef struct xgate_config			xgate_config_t;


typedef void					(*xgate_class_func_t)(xgate_class_t *gclass);

typedef void					(*xgate_config_init_func_t)(xgate_class_config_t *gclass_conf);
typedef void					(*xgate_config_deinit_func_t)(xgate_class_config_t *gclass_conf);
typedef bool_t					(*xgate_config_setup_func_t)(xgate_class_config_t *gclass_conf, const xcommand_arg_t *arg);
typedef void					(*xgate_config_print_func_t)(xgate_class_config_t *gclass_conf, enum xprompt_attr attr);

typedef bool_t					(*xgate_open_func_t)(xgate_object_t *gobj);
typedef bool_t					(*xgate_close_func_t)(xgate_object_t *gobj);
typedef void					(*xgate_poll_func_t)(xgate_object_t *gobj);

typedef void					(*xgate_send_func_t)(xgate_object_t *gobj, const uint8_t *data, size_t data_size);


typedef struct xgate_status
{
	enum xgate_open_status		open_status;

	enum xgate_status_mask		status_mask;

	uint32_t					send_packet_counter;
	uint32_t					recv_packet_counter;
} xgate_status_t;

typedef struct xgate_class_api
{
	xgate_config_init_func_t	config_init;
	xgate_config_deinit_func_t	config_deinit;
	xgate_config_setup_func_t	config_setup;
	xgate_config_print_func_t	config_print;

	xgate_open_func_t			gate_open;
	xgate_close_func_t			gate_close;
	xgate_poll_func_t			gate_poll;

	xgate_send_func_t			gate_send;
} xgate_class_api_t;

typedef struct xgate_class
{
	const char *				name;
	const char *				help;

	size_t						config_size;
	size_t						object_size;

	xgate_class_api_t			api;
} xgate_class_t;

typedef struct xgate_class_config
{
	xgate_class_t *				gclass;
	void *						custom;
} xgate_class_config_t;

typedef struct xgate_class_object
{
	xgate_class_t *				gclass;
	void *						custom;
} xgate_class_object_t;

typedef struct xgate_config
{
	char						name[XGATE_OBJECT_NAME_LEN_MAX + 1];
} xgate_config_t;

typedef struct xgate_object
{
	xgate_class_object_t		class_obj;

	xgate_class_config_t		class_config;
	xgate_class_config_t		class_config_next;

	xgate_config_t				config;
	xgate_status_t				status;

	uint8_t						monitor_channel[XGATE_MONITOR_CONTENTS_MAX];	/* 0: Disable, 1以上: Monitor Channel + 1 */

	uint8_t *					send_buffer;
	size_t						send_buffer_size;

	bool_t						open_req;
	bool_t						reopen_req;

	uint32_t					forwarding_to_gate_cts_mask;	/* 転送先ゲートのClear to Send状態一覧 */
} xgate_object_t;


void							xgate_init(void);
void							xgate_deinit(void);
void							xgate_poll(void);

void							xgate_class_enumeration(void (*callback)(const xgate_class_t *gclass, void *arg), void *arg);
void							xgate_object_enumeration(void (*callback)(uint8_t gate_no, const xgate_object_t *gobj, void *arg), void *arg);

bool_t							xgate_forwarding_rule_get(uint8_t src_gate_no, uint32_t *dst_gate_mask);
bool_t							xgate_forwarding_rule_set(uint8_t src_gate_no, uint32_t dst_gate_mask);

void							xgate_monitor_reset(void);
void							xgate_monitor_clear(uint8_t ch);
bool_t							xgate_monitor_get(uint8_t ch, uint8_t *gate_no, enum xgate_monitor_contents *contents);
bool_t							xgate_monitor_set(uint8_t ch, uint8_t gate_no, enum xgate_monitor_contents contents);

bool_t							xgate_setup(uint8_t gate_no, const char *class_name, const char *obj_name, const char *command);
bool_t							xgate_vsetup(uint8_t gate_no, const char *class_name, const char *obj_name, uint16_t paramc, char * const * paramv);

void							xgate_reset(uint8_t gate_no);

void							xgate_print(uint8_t gate_no, enum xgate_print_contents_mask mask, enum xprompt_attr attr);

void							xgate_open_request(uint8_t gate_no, bool_t open);
enum xgate_open_status			xgate_open_status_get(uint8_t gate_no);

void							xgate_send(uint8_t gate_no, const uint8_t *data, size_t data_size);

void							xgate_object_reopen(xgate_object_t *gobj);

void							xgate_object_send(xgate_object_t *gobj, const uint8_t *data, size_t data_size);
void							xgate_object_recv(xgate_object_t *gobj, const uint8_t *data, size_t data_size);

enum xgate_status_mask			xgate_object_status_get(xgate_object_t *gobj);
void							xgate_object_status_reset(xgate_object_t *gobj, enum xgate_status_mask init_status);
void							xgate_object_status_set(xgate_object_t *gobj, enum xgate_status_mask set_mask);
void							xgate_object_status_clear(xgate_object_t *gobj, enum xgate_status_mask clear_mask);

#endif /* XGATE_H_ */
