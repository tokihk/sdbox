/*
 * xmalloc.h
 *
 *  Created on: 2024/03/13
 *      Author: Hitoshi Kono
 */

#ifndef XMALLOC_H_
#define XMALLOC_H_

#include "core/xstddef.h"


void *							xmalloc(size_t size);
void							xfree(void *ptr);

void *							xcalloc(size_t n, size_t size);


#endif /* XMALLOC_H_ */
