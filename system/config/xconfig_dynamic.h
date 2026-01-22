/*
 * xconfig_dynamic.h
 *
 *  Created on: 2024/04/03
 *      Author: Hitoshi Kono
 */

#ifndef XCONFIG_DYNAMIC_H_
#define XCONFIG_DYNAMIC_H_

#include "core/xstddef.h"

#include "config/xconfig_board.h"


#define XCONFIG_DYNAMIC_VERSION_CODE


typedef struct xconfig_macaddr
{
	uint8_t						addr[6];
} xconfig_macaddr_t;

typedef struct xconfig_security
{

} xconfig_security_t;

typedef struct xconfig_machine
{

} xconfig_machine_t;

typedef struct xconfig_personal
{

} xconfig_personal_t;

typedef struct xconfig_system
{

} xconfig_system_t;

typedef struct xconfig_all
{
	uint8_t						dammy;
} xconfig_all_t;


void							xconfig_dynamic_init(void);


#endif /* XCONFIG_DYNAMIC_H_ */
