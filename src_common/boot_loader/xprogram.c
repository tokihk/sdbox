/*
 * program.c
 *
 *  Created on: 2024/03/12
 *      Author: Hitoshi Kono
 */

#include "xprogram.h"

#include "boot_loader/xboot_loader.h"

#include "prompt/xprompt.h"
#include "prompt/xcommand.h"


void xprogram_main(void)
{
	xprompt_init();

	xcommand_exec(0, "bl select 0");

//	xprogram_system_change();

	for (;;)
	{
		xprompt_poll();
	}
}

void xprogram_shutdown(void)
{
	/* ペリフェラル停止 */
	xprompt_shutdown();
}
