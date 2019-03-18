/*
 * hash.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <stdint.h>
#include <stdlib.h>

#ifndef _HASH_H_
#define _HASH_H_

int accumulation(const char *str);

unsigned int murmur3_32(const char *str);

#endif
