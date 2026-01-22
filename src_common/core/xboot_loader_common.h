/*
 * xboot_loader_common.h
 *
 *  Created on: 2024/03/18
 *      Author: Hitoshi Kono
 */

#ifndef XBOOT_LOADER_COMMON_H_
#define XBOOT_LOADER_COMMON_H_

#include "core/xstddef.h"


#define XBL_IMGPROP_REGION_SIZE		1024

#define XBL_IMGPROP_SIGNATURE		0x64,0x96,0xF0,0xF8,0x5B,0x38,0x1A,0x5B,\
									0x32,0xF2,0xC8,0xDE,0x45,0xC8,0x34,0x79,

#define XBL_IMGPROP_SIGNATURE_SIZE	(16)

#define XBL_IMGPROP_COMMENT_SIZE	(108)


typedef struct xbl_image_property
{
	uint32_t					size;									/* 構造体のサイズ */
	uint8_t						signature[XBL_IMGPROP_SIGNATURE_SIZE];
	uint8_t						comment[XBL_IMGPROP_COMMENT_SIZE];
} xbl_image_property_t;


#endif /* XBOOT_LOADER_COMMON_H_ */
