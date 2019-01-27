/*
 * cstring.c - Corgi DB
 *
 * String type for corgi db.
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <stdlib.h>
#include <string.h>

#include <cstring.h>
#include <error.h>

CString *cstr_create(const char *str) {
  CString *str_node;

  str_node = (CString *) malloc(sizeof(CString));
  if (!str_node) {
    db_error(1, "Failed to allocated memory");
    return NULL;
  }

  // allocate length * 2 memory for buffer
  int length = strlen(str);
  // last byte for '\0'
  char *buffer = (char *) malloc(sizeof(char) * (length * 2 + 1));
  if (!buffer) {
    db_error(1, "Failed to allocated memory");
    return NULL;
  }

  strcpy(buffer, str);
  str_node->size = length * 2;
  str_node->used = length;
  str_node->buffer = buffer;
}

char *cstr_get(CString *node) {
  return node ? node->buffer : NULL;
}

int cstr_set(CString *node, const char *str) {
  if (!node) {
    return 1;
  }

  int length = strlen(str);
  if (length > node->size) {
    return 1;
  }

  strcpy(node->buffer, str);

  return 0;
}
