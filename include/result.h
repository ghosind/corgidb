/*
 * result.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#ifndef _RESULT_H_
#define _RESULT_H_

typedef enum CorgiDBStatusCode {
  // 0x0*** system operational codes
  // 0x00** success codes
  RESULT_OK = 0x0000,
  RESULT_KEY_EXIST = 0x0001,

  // 0x01** user errors
  ERR_USR_TTL = 0x0101,

  // 0x02** command errors
  ERR_NO_COMMAND = 0x0200,
  ERR_NO_KEY = 0x0201,
  ERR_NO_TTL = 0x0202,
  ERR_KEY_EXIST = 0x0203,
  ERR_KEY_NOT_EXIST = 0x0204,

  // 0x03** db errors
  ERR_DB_FULL = 0x0301,

  // 0x1*** system warning codes

  // 0x2*** system error codes
  // 0x20** system error
  ERR_SYS_PARAMS = 0x2001,

  // 0x21** memory error
  ERR_MEM_ALLOC = 0x2101,
  ERR_MEM_REALLOC = 0x2102,

  // 0x22** dictionary error
  ERR_DICT_SIZE = 0x2201,
  ERR_UNKNOWN_RESIZE = 0x2202,

  // 0x23** hash error
  ERR_UNKNOWN_HASH = 0x2301,

  // 0x24** cstring error
  ERR_CSTR_NO_NODE = 0x2401,
  ERR_CSTR_OFFSET_OVER = 0x2402,
} CorgiDBStatusCode;

typedef struct CorgiDBResult {
  CorgiDBStatusCode code;
  int len;
  char **buf;
} CorgiDBResult;

CorgiDBResult *db_result_init(const int len);
int db_result_add(CorgiDBResult *result, const char *str);
int db_result_add_range(CorgiDBResult *result, const char *str, 
    const int length);

#endif
