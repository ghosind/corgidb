/*
 * error.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#ifndef _ERROR_H_
#define _ERROR_H_

enum db_error_code {
  // memory error
  ERR_MEM_ALLOC = 0x01,
  ERR_MEM_REALLOC = 0x02,
};

void db_error(const int error_code, const char *msg, ...);
void db_fatal(const int error_code, const char *msg, ...);

#endif
