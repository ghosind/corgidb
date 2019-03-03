/*
 * error.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <result.h>

#ifndef _ERROR_H_
#define _ERROR_H_

void db_error(const enum CorgiDBStatusCode error_code, const char *msg, ...);
void db_fatal(const enum CorgiDBStatusCode error_code, const char *msg, ...);

#endif
