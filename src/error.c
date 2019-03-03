/*
 * error.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <error.h>

void db_error(const enum CorgiDBStatusCode error_code, 
    const char *message, ...) {
  va_list args;
  
  va_start(args, message);

  errno = (int) error_code;
  vfprintf(stderr, message, args);
}

void db_fatal(const enum CorgiDBStatusCode error_code, 
    const char *message, ...) {
  va_list args;

  va_start(args, message);

  vfprintf(stderr, message, args);
  exit(error_code);
}
