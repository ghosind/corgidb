/*
 * memory.h - Corgi DB
 *
 * Memory tools.
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <stdlib.h>

#ifndef _MEMORY_H_
#define _MEMORY_H_

void *db_malloc(size_t size);
void *db_realloc(void *ptr, size_t size);
void *db_memcpy(void *src, size_t size);

#endif
