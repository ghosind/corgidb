/*
 * corgi.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <corgi.h>
#include <error.h>
#include <dict.h>

CorgiDB *db_init(CorgiDBConfig *config) {
  CorgiDB *db;
  
  db = (CorgiDB *) db_malloc(sizeof(CorgiDB));
  if (!db) {
    return NULL;
  }

  db->config = config;
  db->dict = dict_init();

  return db;
}

int db_set(const CorgiDB *db, const char *key, const char *value, const enum DBSetFlag flag) {
  return dict_set(db->dict, key, value, flag);
}

int db_set_nx(const CorgiDB *db, const char *key, const char *value) {
  return dict_set(db->dict, key, value, SetFlag_NX);
}

char *db_get(const CorgiDB *db, const char *key) {
  char *result = dict_get(db->dict, key);

  if (!result) {
    return NULL;
  }

  int len = strlen(result);
  char *buffer = (char *) malloc(sizeof(char) * (len + 1));

  return strcpy(buffer, result); 
}

int db_delete(const CorgiDB *db, const char *key) {
  return dict_delete(db->dict, key);
}

int db_strlen(const CorgiDB *db, const char *key) {
  char *value = dict_get(db->dict, key);

  if (!value) {
    return -1;
  }

  return strlen(value);
}
