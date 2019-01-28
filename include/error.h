/*
 * error.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#ifndef _ERROR_H_
#define _ERROR_H_

enum DBErrorCode {
  // memory error
  ERR_MEM_ALLOC = 0x0001,
  ERR_MEM_REALLOC = 0x0002,

  // system error
  ERR_SYS_PARAMS = 0x1001,
};

void db_error(const enum DBErrorCode error_code, const char *msg, ...);
void db_fatal(const enum DBErrorCode error_code, const char *msg, ...);

#endif
