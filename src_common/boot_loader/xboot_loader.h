/*
 * xboot_loader.h
 *
 *  Created on: 2024/03/12
 *      Author: Hitoshi Kono
 */

#ifndef XBOOT_LOADER_H_
#define XBOOT_LOADER_H_

#include "core/xstddef.h"

#include "core/xboot_loader_common.h"


enum XBL_BOOT_MODE
{
	XBL_BOOT_MODE_NONE,
	XBL_BOOT_MODE_DIRECT,
};


typedef struct xbl_image_info
{
	xbl_image_property_t		prop;
	uint32_t					contents_addr;
	bool_t						is_valid;
} xbl_image_info_t;


bool_t							xbl_image_info_get(uint8_t image_index, xbl_image_info_t *image_info);

bool_t							xbl_system_load_from_image(const xbl_image_info_t *image_info);

bool_t							xbl_system_run(void);


#endif /* XBOOT_LOADER_H_ */
