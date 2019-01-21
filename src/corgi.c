/*
 * corgi.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <errno.h>
#include <stdlib.h>

#include <corgi.h>
#include <error.h>
#include <dict.h>

CorgiDB *init() {
  CorgiDB *db;
  
  db = (CorgiDB *) malloc(sizeof(CorgiDB));
  if (!db) {
    db_error(1, "failed to allocate memory for database");

    return NULL;
  }

  db->dict = dict_init();

  return db;
}

int set(const CorgiDB *db, const char *key, const char *value) {
  return dict_set(db->dict, key, value);
}

char *get(const CorgiDB *db, const char *key) {
  return dict_get(db->dict, key);
}