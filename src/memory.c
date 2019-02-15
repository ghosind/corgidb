/*
 * memory.c - Corgi DB
 *
 * Memory tools.
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <stdlib.h>
#include <string.h>

#include <error.h>
#include <memory.h>

void *db_malloc(size_t size) {
  void *memory = malloc(size);

  if (!memory) {
    db_error(ERR_MEM_ALLOC, "Failed to allocate memory,");
    return NULL;
  }

  return memory;
}

void* db_realloc(void* ptr, size_t size) {
  void *memory = realloc(ptr, size);

  if (!memory) {
    db_error(ERR_MEM_REALLOC, "Failed to re-allocate memory.");
    return NULL;
  }

  return memory;
}

void *db_memcpy(void *src, size_t size) {
  void *mem = db_malloc(size);
  if (!mem) {
    return NULL;
  }

  memcpy(mem, src, size);

  return mem;
}
