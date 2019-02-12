/*
 * result.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <memory.h>
#include <result.h>

#include <string.h>

CorgiDBResult *db_result_init(int len) {
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

int db_result_add(CorgiDBResult *result,  char *str) {
  int length = strlen(str);
  char *buffer = (char *) db_malloc(sizeof(char) * (length + 1));

  if (!buffer) {
    return 1;
  }

  strcpy(buffer, str);
  result->buf[result->len] = buffer;
  result->len++;

  return 0;
}
