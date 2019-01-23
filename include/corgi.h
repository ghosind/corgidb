/*
 * corgi.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <dict.h>

#ifndef _CORGI_H_
#define _CORGI_H_

typedef struct CorgiDB {
  Dict *dict;
} CorgiDB;

// corgidb systems
CorgiDB *init();

// corgidb commands
int set(const CorgiDB *db, const char *key, const char *value);
char *get(const CorgiDB *db, const char *key, char *buf);
int del(const CorgiDB *db, const char *key);

#endif
