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
#include <result.h>

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

CorgiDBResult *db_resize(CorgiDB *db, const unsigned int size) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  result->code = dict_resize(db->dict, size);

  return result;
}

CorgiDBResult *db_keys(const CorgiDB *db) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  char **keys = dict_keys(db->dict);

  for (int i = 0; i < db->dict->used; i++) {
    db_result_add(result, *(keys + i));
  }

  return result;
}

CorgiDBResult *db_set(const CorgiDB *db, const char *key, const char *value, 
    const enum DBSetFlag flag, const long ttl) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  result->code = dict_set(db->dict, key, value, flag, ttl);

  return result;
}

CorgiDBResult *db_set_ex(const CorgiDB *db, const char *key, const char *value,
    const long ttl) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  result->code = dict_set(db->dict, key, value, SetFlag_NONE, ttl);

  return result;
}

CorgiDBResult *db_set_nx(const CorgiDB *db, const char *key, 
    const char *value) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }
  
  result->code = dict_set(db->dict, key, value, SetFlag_NX, 0);

  return result;
}

CorgiDBResult *db_get(const CorgiDB *db, const char *key) {
  CorgiDBResult *result = db_result_init(1);
  if (!result) {
    return NULL;
  }
  
  char *str = dict_get(db->dict, key);

  if (str) {
    db_result_add(result, str);
  }

  return result;
}

CorgiDBResult *db_get_range(const CorgiDB *db, const char *key, 
  const unsigned int start, const unsigned int end) {
  CorgiDBResult *result = db_result_init(1);
  if (!result) {
    return NULL;
  }
  
  DictNode *node = dict_find(db->dict, key);

  if (node) {
    int length = 0;
    char *str = cstr_get_range(node->value, start - 1, end - 1, &length);
    db_result_add_range(result, str, length);
  }

  return result;
}

CorgiDBResult *db_getset(const CorgiDB *db, const char *key, 
    const char *value) {
  CorgiDBResult *result = db_get(db, key);
  
  if (!result || result->code != RESULT_OK) {
    return result;
  }

  result->code = dict_set(db->dict, key, value, SetFlag_NONE, 0);
  
  return result;
}

CorgiDBResult *db_set_range(const CorgiDB *db, const char *key, 
    const char *value, const int offset) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  DictNode *node = dict_find(db->dict, key);

  if (!node) {
    result->code = ERR_NO_KEY;
  } else {
    result->len = cstr_set_range(node->value, value, offset - 1);
    result->code = RESULT_OK;
  }

  return result;
}

CorgiDBResult *db_mset(const CorgiDB *db, const char ***kv_pairs, 
    const int len, const enum DBSetFlag flag, const long ttl) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }
  
  trans_begin(db->dict);

  for (int i = 0; i < len; i++) {
    int code = dict_set(db->dict, kv_pairs[i][0], kv_pairs[i][1], flag, ttl);

    if (code) {
      trans_rollback(db->dict);
      result->code = code;
      return result;
    }
  }

  trans_commit(db->dict);

  result->code = RESULT_OK;
  
  return result;
}

CorgiDBResult *db_mset_ex(const CorgiDB *db, const char ***kv_pairs, 
    const int len, const long ttl) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  trans_begin(db->dict);

  for (int i = 0; i < len; i++) {
    int code = dict_set(db->dict, kv_pairs[i][0], kv_pairs[i][1], 
        SetFlag_NONE, ttl);

    if (code) {
      trans_rollback(db->dict);
      result->code = code;
      return result;
    }
  }

  trans_commit(db->dict);

  result->code = RESULT_OK;

  return result;
}

CorgiDBResult *db_mset_nx(const CorgiDB *db, const char ***kv_pairs, 
    const int len) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  for (int i = 0; i < len; i++) {
    int exists = dict_key_exist(db->dict, kv_pairs[i][0]);

    if (exists) {
      result->code = ERR_KEY_EXIST;
      return result;
    }
  }

  trans_begin(db->dict);

  for (int i = 0; i < len; i++) {
    int code = dict_set(db->dict, kv_pairs[i][0], kv_pairs[i][1], 
        SetFlag_NX, 0);

    if (code) {
      trans_rollback(db->dict);
      result->code = code;
      return result;
    }
  }

  trans_commit(db->dict);

  result->code = RESULT_OK;

  return result;
}

CorgiDBResult *db_mget(const CorgiDB *db, const char **keys, const int len) {
  CorgiDBResult *result = db_result_init(len);
  if (!result) {
    return NULL;
  }

  for (int i = 0; i < len; i++) {
    char *str = dict_get(db->dict, keys[i]);
    if (!str) {
      continue;
    }

    if (!str) {
      db_result_add(result, str);
    }
  }

  return result;
}

CorgiDBResult *db_delete(const CorgiDB *db, const char *key) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  result->code = dict_delete(db->dict, key);
  
  return result;
}

CorgiDBResult *db_strlen(const CorgiDB *db, const char *key) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  DictNode *node = dict_find(db->dict, key);

  if (!node) {
    result->code = ERR_NO_KEY;
  } else {
    result->len = cstr_len(node->value);
    result->code = RESULT_OK;
  }

  return result;
}

CorgiDBResult *db_exists(const CorgiDB *db, const char **keys, const int len) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  int num = 0;

  for (int i = 0; i < len; i++) {
    num += dict_find(db->dict, keys[i]) == NULL ? 0 : 1;
  }

  result->code = RESULT_OK;
  result->len = num;

  return result;
}

CorgiDBResult *db_append(const CorgiDB *db, const char *key, 
    const char *value) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }
  
  DictNode *node = dict_find(db->dict, key);
  if (!node) {
    result->code = ERR_NO_KEY;
    return result;
  }

  result->code = cstr_append(node->value, value);

  return result;
}

CorgiDBResult *db_ttl(const CorgiDB *db, const char *key) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  DictNode *node = dict_find(db->dict, key);
  if (!node) {
    result->code = ERR_NO_KEY;
    return result;
  }

  if (node->expire == 0) {
    result->code = ERR_NO_TTL;
    return result;
  }

  result->len = node->expire - time(NULL);
  result->code = RESULT_OK;

  return result;
}

CorgiDBResult *db_expire(const CorgiDB *db, const char *key, const long ttl) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  if (ttl <= 0) {
    result->code = ERR_USR_TTL;
    return result;
  }

  DictNode *node = dict_find(db->dict, key);
  if (!node) {
    result->code = ERR_NO_KEY;
    return result;
  }

  node->expire = time(NULL) + ttl;
  result->code = RESULT_OK;

  return result;
}

CorgiDBResult *db_persist(const CorgiDB *db, const char *key) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  DictNode *node = dict_find(db->dict, key);
  if (!node) {
    result->code = ERR_NO_KEY;
    return result;
  }

  node->expire = 0;
  result->code = RESULT_OK;

  return result;
}

CorgiDBResult *db_flush(const CorgiDB *db) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  dict_flush(db->dict);
  result->code = RESULT_OK;

  return result;
}

CorgiDBResult *db_begin(const CorgiDB *db) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  result->code = trans_begin(db->dict);
  
  return result;
}

CorgiDBResult *db_commit(const CorgiDB *db) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  result->code = trans_commit(db->dict);

  return result;
}

CorgiDBResult *db_rollback(const CorgiDB *db) {
  CorgiDBResult *result = db_result_init(db->dict->used);
  if (!result) {
    return NULL;
  }

  result->code = trans_rollback(db->dict);

  return result;
}
