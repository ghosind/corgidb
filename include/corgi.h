/*
 * corgi.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <config.h>
#include <dict.h>
#include <result.h>

#ifndef _CORGI_H_
#define _CORGI_H_

typedef struct CorgiDB {
  Dict *dict;
  CorgiDBConfig *config;
} CorgiDB;

// corgidb systems
CorgiDB *db_init(CorgiDBConfig *config);
int db_resize(CorgiDB *db, const unsigned int size);

// corgidb commands
CorgiDBResult *db_keys(const CorgiDB *db);
int db_set(const CorgiDB *db, const char *key, const char *value, 
    const enum DBSetFlag flag, const long ttl);
int db_set_ex(const CorgiDB *db, const char *key, const char *value, 
    const long ttl);
int db_set_nx(const CorgiDB *db, const char *key, const char *value);
CorgiDBResult *db_get(const CorgiDB *db, const char *key);
int db_mset(const CorgiDB *db, const char ***kv_pairs, const int len, 
    const enum DBSetFlag flag, const long ttl);
int db_mset_ex(const CorgiDB *db, const char ***kv_pairs, const int len, 
    const long ttl);
int db_mset_nx(const CorgiDB *db, const char ***kv_pairs, const int len);
CorgiDBResult *db_mget(const CorgiDB *db, const char **keys, const int len);
int db_delete(const CorgiDB *db, const char *key);
int db_strlen(const CorgiDB *db, const char *key);
int db_exists(const CorgiDB *db, const char **keys, const int len);
int db_append(const CorgiDB *db, const char *key, const char *value);
int db_ttl(const CorgiDB *db, const char *key);
int db_expire(const CorgiDB *db, const char *key, const long ttl);
int db_persist(const CorgiDB *db, const char *key);
int db_flush(const CorgiDB *db);
int db_begin(const CorgiDB *db);
int db_commit(const CorgiDB *db);
int db_rollback(const CorgiDB *db);

#endif
