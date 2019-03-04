/*
 * result.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <memory.h>
#include <result.h>

#include <string.h>

CorgiDBResult *db_result_init(const int len) {
  CorgiDBResult *result = (CorgiDBResult *) db_malloc(sizeof(CorgiDBResult));
  if (!result) {
    return NULL;
  }

  result->code = RESULT_OK;
  result->len = 0;

  if (len != 0) {
    char **buffers = (char **) db_malloc(sizeof(char *) * len);
    if (!buffers) {
      free(result);
      return NULL;
    }
  
    result->buf = buffers;
  }

  return result;
}

int db_result_add(CorgiDBResult *result, const char *str) {
  int length = strlen(str);
  char *buffer = (char *) db_malloc(sizeof(char) * (length + 1));

  if (!buffer) {
    return ERR_MEM_ALLOC;
  }

  strcpy(buffer, str);
  result->buf[result->len] = buffer;
  result->len++;

  return RESULT_OK;
}

int db_result_add_range(CorgiDBResult *result, const char *str, 
    const int length) {
  char *buffer = (char *) db_malloc(sizeof(char) * (length + 1));

  if (!buffer) {
    return ERR_MEM_ALLOC;
  }

  int i = 0;
  for (; i < length; i++) {
    buffer[i] = str[i];
  }
  buffer[i] = '\0';
  result->buf[result->len] = buffer;
  result->len++;

  return RESULT_OK;
}
