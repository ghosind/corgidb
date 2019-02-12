/*
 * result.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#ifndef _RESULT_H_
#define _RESULT_H_

enum CorgiDBResultCode {
  RESULT_OK = 0,
};

typedef struct CorgiDBResult {
  enum CorgiDBResultCode code;
  int len;
  char **buf;
} CorgiDBResult;

CorgiDBResult *db_result_init(int len);
int db_result_add(CorgiDBResult *result,  char *str);

#endif
