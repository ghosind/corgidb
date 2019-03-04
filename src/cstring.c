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
#include <memory.h>

CString *cstr_create(const char *str) {
  CString *str_node;

  str_node = (CString *) db_malloc(sizeof(CString));
  if (!str_node) {
    return NULL;
  }

  // allocate length * 2 memory for buffer
  int length = strlen(str);
  // last byte for '\0'
  char *buffer = (char *) db_malloc(sizeof(char) * (length + 1));
  if (!buffer) {
    return NULL;
  }

  strcpy(buffer, str);
  str_node->size = length;
  str_node->used = length;
  str_node->buffer = buffer;

  return str_node;
}

char *cstr_get(CString *node) {
  return node ? node->buffer : NULL;
}

char *cstr_get_range(CString *node, const unsigned int start, 
    const unsigned int end, int *length) {
  if (start > node->used || start > end) {
    return NULL;
  }

  *length = (end > node->used ? node->used : end) - start + 1;
  return node->buffer + start;
}

int cstr_set(CString *node, const char *str) {
  if (!node) {
    return ERR_CSTR_NO_NODE;
  }

  int length = strlen(str);

  if (length > node->size) {
    char *new_buffer = (char *) db_realloc(node->buffer, length + 1);
    if (!new_buffer) {
      return ERR_MEM_REALLOC;
    }

    node->buffer = new_buffer;
    node->size = length;
  }

  strcpy(node->buffer, str);
  node->used = length;

  return RESULT_OK;
}

int cstr_set_range(CString *node, const char *str, int offset) {
  if (offset < 0) {
    return ERR_CSTR_OFFSET_OVER;
  }

  int length = strlen(str);

  if (length + offset > node->size) {
    char *new_buffer = (char *) db_realloc(node->buffer, length + offset + 1);
    if (!new_buffer) {
      return ERR_MEM_REALLOC;
    }

    node->buffer = new_buffer;
    node->size = length + offset;
  }

  strcpy(node->buffer + offset, str);
  node->used = length + offset;

  return RESULT_OK;
}

void cstr_free(CString *node) {
  free(node->buffer);
  free(node);
}

int cstr_len(CString *node) {
  return node->used;
}

int cstr_is_equal(CString *node, const char *str) {
  if (node->used != strlen(str)) {
    return 0;
  }

  return !strcmp(node->buffer, str);
}

int cstr_cmp(CString *node, const char *str) {
  return strcmp(node->buffer, str);
}

int cstr_append(CString *node, const char *str) {
  int length = strlen(str);

  if (node->used + length > node->size) {
    char *new_buffer = (char *) db_realloc(node->buffer, node->used + length + 1);
    if (!new_buffer) {
      return ERR_MEM_REALLOC;
    }

    node->buffer = new_buffer;
    node->size = node->used + length;
  }

  node->buffer = strcat(node->buffer, str);
  node->used = node->used + length;

  return node->used;
}