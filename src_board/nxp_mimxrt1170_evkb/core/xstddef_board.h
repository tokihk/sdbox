/*
 * xstddef_board.h
 *
 *  Created on: 2024/03/12
 *      Author: Hitoshi Kono
 */

#ifndef XSTDDEF_BOARD_H_
#define XSTDDEF_BOARD_H_

#if 0
#define XEI()								__enable_irq()
#define XDI()								__disable_irq()

#ifndef XSECTION_NONCACHE_DATA
#define XSECTION_NONCACHE_DATA(var)			__attribute__((section("NonCacheable,\"aw\",%nobits @"))) var __attribute__((aligned(64)))
#endif

#ifndef XSECTION_NONCACHE_DATA_INIT
#define XSECTION_NONCACHE_DATA_INIT(var)	__attribute__((section("NonCacheable.init"))) var __attribute__((aligned(64)))
#endif

#ifndef XSECTION_QUICKACCESS_FUNC
#define XSECTION_QUICKACCESS_FUNC(func)		__attribute__((section("CodeQuickAccess"), __noinline__)) func
#endif

#ifndef XSECTION_QUICKACCESS_DATA
#define XSECTION_QUICKACCESS_DATA(var)		__attribute__((section("DataQuickAccess"))) var __attribute__((aligned(4)))
#endif

#endif

#ifndef XSECTION_BOOT_IMAGE_PROPERTY
#define XSECTION_BOOT_IMAGE_PROPERTY(var)	__attribute__ ((section(".XBootImageProperty"), used)) __attribute__ ((aligned (4))) var
#endif

#endif /* XSTDDEF_BOARD_H_ */
