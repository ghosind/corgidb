/*
 * hash.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <error.h>
#include <hash.h>

#include <string.h>

unsigned int murmur3_32(const char* str) {
  unsigned int h = 0;
  size_t len = strlen(str);

  if (len > 3) {
    const uint32_t* key_x4 = (const uint32_t*) str;
    size_t i = len >> 2;

    do {
      uint32_t k = *key_x4++;
      k *= 0xcc9e2d51;
      k = (k << 15) | (k >> 17);
      k *= 0x1b873593;
      h ^= k;
      h = (h << 13) | (h >> 19);
      h = (h * 5) + 0xe6546b64;
    } while (--i);

    str = (const char*) key_x4;
  }
  if (len & 3) {
    size_t i = len & 3;
    uint32_t k = 0;
    str = &str[i - 1];

    do {
      k <<= 8;
      k |= *str--;
    } while (--i);

    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    h ^= k;
  }

  h ^= len;
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}
