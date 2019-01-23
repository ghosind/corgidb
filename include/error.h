/*
 * error.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#ifndef _ERROR_H_
#define _ERROR_H_

void db_error(const int error_code, const char *msg, ...);
void db_fatal(const int error_code, const char *msg, ...);

#endif
