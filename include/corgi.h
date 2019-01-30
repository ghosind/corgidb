/*
 * corgi.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <config.h>
#include <dict.h>

#ifndef _CORGI_H_
#define _CORGI_H_

typedef struct CorgiDB {
  Dict *dict;
  CorgiDBConfig *config;
} CorgiDB;

// corgidb systems
CorgiDB *db_init(CorgiDBConfig *config);

// corgidb commands
int db_set(const CorgiDB *db, const char *key, const char *value, const enum DBSetFlag flag);
int db_set_nx(const CorgiDB *db, const char *key, const char *value);
char *db_get(const CorgiDB *db, const char *key);
int db_mset(const CorgiDB *db, const char **kv_pairs, const int len, const enum DBSetFlag flag);
int db_mset_nx(const CorgiDB *db, const char *kv_pairs, const int len);
int db_delete(const CorgiDB *db, const char *key);
int db_strlen(const CorgiDB *db, const char *key);

#endif
