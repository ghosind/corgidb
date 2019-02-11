/*
 * corgi.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <corgi.h>
#include <cstring.h>
#include <error.h>
#include <dict.h>
#include <memory.h>

CorgiDB *db_init(CorgiDBConfig *config) {
  CorgiDB *db;
  
  db = (CorgiDB *) db_malloc(sizeof(CorgiDB));
  if (!db) {
    return NULL;
  }

  db->config = config;
  db->dict = dict_init(config);

  return db;
}

int db_resize(CorgiDB *db, const unsigned int size) {
  return dict_resize(db->dict, size);
}

int db_set(const CorgiDB *db, const char *key, const char *value, 
    const enum DBSetFlag flag, const long ttl) {
  return dict_set(db->dict, key, value, flag, ttl);
}

int db_set_nx(const CorgiDB *db, const char *key, const char *value, const long ttl) {
  return dict_set(db->dict, key, value, SetFlag_NX, ttl);
}

char *db_get(const CorgiDB *db, const char *key) {
  char *result = dict_get(db->dict, key);

  if (!result) {
    return NULL;
  }

  int len = strlen(result);
  char *buffer = (char *) db_malloc(sizeof(char) * (len + 1));
  if (!buffer) {
    return NULL;
  }

  return strcpy(buffer, result); 
}

int db_mset(const CorgiDB *db, const char ***kv_pairs, const int len, 
    const enum DBSetFlag flag, const long ttl) {
  trans_begin(db->dict);

  for (int i = 0; i < len; i++) {
    int result = dict_set(db->dict, kv_pairs[i][0], kv_pairs[i][1], flag, ttl);

    if (result) {
      trans_rollback(db->dict);
      return result;
    }
  }

  trans_commit(db->dict);
  return 0;
}

int db_mset_nx(const CorgiDB *db, const char ***kv_pairs, const int len, const long ttl) {
  trans_begin(db->dict);

  for (int i = 0; i < len; i++) {
    int result = dict_set(db->dict, kv_pairs[i][0], kv_pairs[i][1], SetFlag_NX, ttl);

    if (result) {
      trans_rollback(db->dict);
      return result;
    }
  }

  trans_commit(db->dict);
  return 0;
}

CorgiDBResult *db_mget(const CorgiDB *db, const **keys, const int len) {
  CorgiDBResult *result = (CorgiDBResult *) db_malloc(sizeof(CorgiDBResult));
  if (!result) {
    return NULL;
  }

  char **buffers = (char **) db_malloc(sizeof(char *) * len);
  if (!buffers) {
    return NULL;
  }

  result->code = 0;
  result->len = 0;
  result->buf = buffers;

  for (int i = 0, j = 0; i < len && j <= i; i++) {
    char *str = dict_get(db->dict, keys[i]);
    if (!str) {
      continue;
    }

    int str_len = strlen(result);
    char *buffer = (char *) db_malloc(sizeof(char) * (str_len + 1));
    if (!buffer) {
      return NULL;
    }

    strcpy(buffer, str);
    result->buf[j] = buffer;
    result->len++;
  }

  return result;
}

int db_delete(const CorgiDB *db, const char *key) {
  return dict_delete(db->dict, key);
}

int db_strlen(const CorgiDB *db, const char *key) {
  DictNode *node = dict_find(db->dict, key, NULL);

  if (!node) {
    return -1;
  }

  return cstr_len(node->value);
}

int db_exists(const CorgiDB *db, const char **keys, const int len) {
  int result = 0;

  for (int i = 0; i < len; i++) {
    result += dict_find(db->dict, keys[i], NULL) ? 1 : 0;
  }

  return result;
}

int db_append(const CorgiDB *db, const char *key, const char *value) {
  DictNode *node = dict_find(db->dict, key, NULL);
  if (!node) {
    return -1;
  }

  return cstr_append(node->value, value);
}

int db_ttl(const CorgiDB *db, const char *key) {
  DictNode *node = dict_find(db->dict, key, NULL);
  if (!node) {
    return -1;
  }

  if (node->expire == 0) {
    return -2;
  }

  return node->expire - time(NULL);
}

int db_expire(const CorgiDB *db, const char *key, const long ttl) {
  if (ttl <= 0) {
    return -2;
  }

  DictNode *node = dict_find(db->dict, key, NULL);
  if (!node) {
    return -1;
  }

  node->expire = time(NULL) + ttl;
  return 0;
}

int db_persist(const CorgiDB *db, const char *key) {
  DictNode *node = dict_find(db->dict, key, NULL);
  if (!node) {
    return -1;
  }

  node->expire = 0;
  return 0;
}

int db_begin(const CorgiDB *db) {
  return trans_begin(db->dict);
}

int db_commit(const CorgiDB *db) {
  return trans_commit(db->dict);
}

int db_rollback(const CorgiDB *db) {
  return trans_rollback(db->dict);
}
