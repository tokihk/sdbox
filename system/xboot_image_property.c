/*
 * xboot_image_property.c
 *
 *  Created on: 2024/03/19
 *      Author: Hitoshi Kono
 */

#include "core/xboot_loader_common.h"

XSECTION_BOOT_IMAGE_PROPERTY(const xbl_image_property_t XBOOT_IMAGE_PROPERTY_DATA);

const xbl_image_property_t XBOOT_IMAGE_PROPERTY_DATA =
{
	.size 		= sizeof(xbl_image_property_t),
	.signature	= { XBL_IMGPROP_SIGNATURE },
	.comment	= "SDSGW (Software Defined Slim GateWay)",
};
