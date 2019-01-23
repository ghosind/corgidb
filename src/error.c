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

void db_error(const int error_code, const char *message, ...) {
  va_list args;
  
  va_start(args, message);

  errno = error_code;
  vfprintf(stderr, message, args);
}

void db_fatal(const int error_code, const char *message, ...) {
  va_list args;

  va_start(args, message);

  vfprintf(stderr, message, args);
  exit(error_code);
}
