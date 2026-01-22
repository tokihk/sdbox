/*
 * xboot_loader.c
 *
 *  Created on: 2024/03/12
 *      Author: Hitoshi Kono
 */

#include "boot_loader/xboot_loader.h"

#include "xprogram_main.h"

#include <string.h>


typedef struct xbl_system_info
{
	uint32_t					start_addr;
	uint32_t					vectbl_addr;
	uint32_t					stack_addr;
} xbl_system_info_t;


#include "boot_loader/xboot_loader_board.h"


static const uint8_t XBL_IMAGE_SIGNATURE[] = { XBL_IMGPROP_SIGNATURE };

static struct
{
	xbl_system_info_t			system_info;
} g_xboot_loader;


bool_t xbl_image_info_get(uint8_t image_index, xbl_image_info_t *image_info)
{
	bool_t get_ok = FALSE;

	if (image_info != NULL)
	{
		uint32_t image_addr;
		uint32_t image_prop_addr;

		memset(image_info, 0, sizeof(*image_info));

		/* イメージの配置アドレス取得 */
		if (xbl_image_addr_get_board(image_index, &image_addr, &image_prop_addr))
		{
			const uint8_t *image = (const uint8_t *)image_addr;
			const xbl_image_property_t *prop = (const xbl_image_property_t *)image_prop_addr;

			/* シグネチャコードを確認 */
			if (   (prop->size >= (XOFFSETOF(xbl_image_property_t, signature) + sizeof(prop->signature)))
				&& (memcmp(prop->signature, XBL_IMAGE_SIGNATURE, sizeof(prop->signature)) == 0)
			){
				image_info->is_valid = TRUE;
			}

			/* 情報取得 */
			if (image_info->is_valid)
			{
				image_info->prop = *prop;
				image_info->prop.comment[XCOUNTOF(image_info->prop.comment) - 1] = '\0';
				image_info->contents_addr = image_addr;
			}

			get_ok = TRUE;
		}
	}

	return (get_ok);
}

bool_t xbl_system_load_from_image(const xbl_image_info_t *image_info)
{
	bool_t load_ok = FALSE;

	if ((image_info != NULL) && (image_info->is_valid)
	){
		/* イメージ情報からシステム情報を取得 */
		xbl_system_info_get_board(image_info->contents_addr, &g_xboot_loader.system_info);

		load_ok = TRUE;
	}

	return (load_ok);
}

bool_t xbl_system_run(void)
{
	xprogram_shutdown();

	xbl_system_run_board
	(
		g_xboot_loader.system_info.start_addr,
		g_xboot_loader.system_info.vectbl_addr,
		g_xboot_loader.system_info.stack_addr
	);

	/* システム起動に成功した場合はここまで来ない */
	return (FALSE);
}


