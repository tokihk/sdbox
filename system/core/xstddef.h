/*
 * xstddef.h
 *
 *  Created on: 2024/03/12
 *      Author: Hitoshi Kono
 */

#ifndef XSTDDEF_H_
#define XSTDDEF_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "xboard_conf.h"

#include "config/xconfig_const.h"

#include "core/xstddef_board.h"

#include "core/xstdbit.h"


typedef uint8_t								bool_t;


#ifndef TRUE
#define TRUE								((bool_t)1)
#endif

#ifndef FALSE
#define FALSE								((bool_t)0)
#endif

#ifndef XCHAR_IS_WCHAR
#define XCHAR_IS_WCHAR						(0)
#endif

#if XCHAR_IS_WCHAR != 0
	typedef wchar_t							xchar_t;
	#define XTEXT_(text)					L ## text
	#define XVAL2TEXT(v)					L#v
#else
	typedef char							xchar_t;
	#define XTEXT_(text)					text
	#define XVAL2TEXT(v)					#v
#endif

#define XTEXT(text)							XTEXT_(text)

#define XUNUSED(var)						(void)var

#define XCOUNTOF(arr)						(sizeof(arr) / sizeof((arr)[0]))
#define XOFFSETOF(st,name)					((size_t)(&((st *)0)->name))
#define XMEMBER_SIZEOF(st,name)				(size_t)(sizeof(((st *)0)->name))

#define XMIN(v1,v2)							(((v1) < (v2)) ? (v1) : (v2))
#define XMAX(v1,v2)							(((v1) > (v2)) ? (v1) : (v2))
#define XABS(v)								(((v) < 0) ? (-(v)) : (v))

#define XBITSET(v,bitno)					do { ((v) |= (1u << (bitno))); } while (0)
#define XBITCLR(v,bitno)					do { ((v) &= ~(1u << (bitno))); } while (0)

#define XBIT2BYTE(v)						(((v) + 7) / 8)
#define XBIT2DWORD(v)						(((v) + 31) / 32)

#define XTICKDIFF(prev,curr)				(((prev) <= (curr)) ? ((curr) - (prev)) : (0xFFFFFFFFu - (prev) + (curr) + 1))

#define XFOREACH_ARRAY_BEGIN(var,arr)		for ((var) = &((arr)[0]); (var) <= &((arr)[XCOUNTOF(arr) - 1]); (var)++)
#define XFOREACH_ARRAY_END()

#define XFOREACH_PTR_BEGIN(var,arr,arr_n) 	{ \
											size_t foreach_count = 0; \
											for ((var) = (arr); foreach_count < (size_t)(arr_n); (var)++, foreach_count++)
#define XFOREACH_PTR_END()					}

#define XTIMEOUT_INFINITE					(0xFFFFFFFFU)

#ifndef XSECTION_NONCACHE_DATA
#define XSECTION_NONCACHE_DATA(var)			__attribute__ ((section("NonCacheData"))) __attribute__ ((aligned (64))) var
#endif

#ifndef XSECTION_NONCACHE_DATA_INIT
#define XSECTION_NONCACHE_DATA_INIT(var)	__attribute__ ((section("NonCacheData.init"))) __attribute__ ((aligned (64))) var
#endif

#ifndef XSECTION_QUICKACCESS_FUNC
#define XSECTION_QUICKACCESS_FUNC(func)		__attribute__ ((section("QuickAccessFunc"), __noinline__)) func
#endif

#ifndef XSECTION_QUICKACCESS_DATA
#define XSECTION_QUICKACCESS_DATA(var)		__attribute__ ((section("QuickAccessData"))) __attribute__ ((aligned (4))) var
#endif

#endif /* XSTDDEF_H_ */
