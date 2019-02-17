/*
 * dict.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <time.h>

#include <config.h>
#include <cstring.h>
#include <transaction.h>

#ifndef _DICT_H_
#define _DICT_H_

#define get_hash(dict, key) (dict->hash_function(key) % dict->mask)

typedef struct DictNode {
  CString *key;
  CString *value;
  time_t expire;
  struct DictNode *next;
} DictNode;

typedef struct Dict {
  int size;
  int mask;
  int used;
  DictNode **table;
  DictTransaction *transaction;
  int (*hash_function)(const char *key);
} Dict;

enum DBSetFlag {
  SetFlag_NONE = 0,
  SetFlag_XX = 1,
  SetFlag_NX = 2,
};

Dict *dict_init(CorgiDBConfig *config);
void dict_reset(Dict *dict);

int dict_resize(Dict *dict, const int size);
DictNode *dict_find(Dict *dict, const char *key, DictNode **prev);

char *dict_get(Dict *dict, const char *key);
int dict_set(Dict *dict, const char *key, const char *value, 
    const enum DBSetFlag flag, const long ttl);
int dict_delete(Dict *dict, const char *key);
char **dict_keys(Dict *dict);

#endif
