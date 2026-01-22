/*
 * xcommand_bl.c
 *
 *  Created on: 2024/03/14
 *      Author: Hitoshi Kono
 */

#include "xcommand.h"
#include "xprompt.h"

#include "boot_loader/xboot_loader.h"


/* --------------------------------------------------------
 * bl list
 * ----------------------------------------------------- */

const char XCOMMAND_BL_LIST_HELP[] =
{
		"System image list.\n"
};

static bool_t xcommand_bl_list(xcommand_callstack_t *cs)
{
	bool_t				success = TRUE;
	xbl_image_info_t	image_info;
	uint8_t				image_index;
	bool_t				is_exist = FALSE;

	for (image_index = 0; xbl_image_info_get(image_index, &image_info); image_index++)
	{
		if (image_info.is_valid)
		{
			xprompt_printf(cs->prompt_attr, "%d: %s", image_index, image_info.prop.comment);
			is_exist = TRUE;
		}
	}

	/* イメージが一つも見つからなかったとき */
	if (!is_exist)
	{
		xprompt_printf(cs->prompt_attr, "System image is not exist.");
	}

	return (success);
}

/* --------------------------------------------------------
 * bl select [0-x]
 * ----------------------------------------------------- */

const char XCOMMAND_BL_SELECT_HELP[] =
{
		"System image select.\n"
};

static bool_t xcommand_bl_select(xcommand_callstack_t *cs)
{
	bool_t			success = TRUE;
	const char *	argv_text;

	uint8_t			image_index;

	/* 0: image_index */
	if ((success) && (success = xcommand_arg_pop_text(cs, &argv_text)))
	{
		image_index = (uint8_t)atoi(argv_text);
	}

	if (success)
	{
		xbl_image_info_t image_info;

		if (!xbl_image_info_get(image_index, &image_info))
		{
			/* イメージ情報の取得失敗(イメージが存在しない) */
			xprompt_printf(cs->prompt_attr, "System image[%d] is not exist.", image_index);
		}
		else if (!xbl_system_load_from_image(&image_info))
		{
			/* イメージからシステムロードに失敗 */
			xprompt_printf(cs->prompt_attr, "System load failed.");
		}
		else
		{
			/* 読み込み成功 */
			xprompt_printf(cs->prompt_attr, "System loaded successfully.");
		}
	}

	return (success);
}

/* --------------------------------------------------------
 * bl run [0-x]
 * ----------------------------------------------------- */

const char XCOMMAND_BL_RUN_HELP[] =
{
		"System run.\n"
};

static bool_t xcommand_bl_run(xcommand_callstack_t *cs)
{
	if (!xbl_system_run())
	{
		/* システム起動に失敗 */
		xprompt_printf(cs->prompt_attr, "System start failed.");
	}

	return (TRUE);
}

/* =======================================================================
 * 		Command Map (test)
 * ==================================================================== */

const struct xcommand_info XCMDLIST_BL[] =
{
	{ "list",    xcommand_bl_list,    XCOMMAND_BL_LIST_HELP   },
	{ "select",  xcommand_bl_select,  XCOMMAND_BL_SELECT_HELP },
	{ "run",     xcommand_bl_run,     XCOMMAND_BL_RUN_HELP    },
};

bool_t xcommand_bl(xcommand_callstack_t *cs)
{
	return (xcommand_sub_call(cs, XCMDLIST_BL, XCOUNTOF(XCMDLIST_BL)));
}


