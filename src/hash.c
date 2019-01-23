/*
 * hash.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <error.h>

// test function
int accumulation(const char *str) {
  int hash_value = 0;

  while (*str) {
    hash_value += *str;
    str++;
  }

  return hash_value;
}
