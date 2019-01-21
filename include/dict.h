/*
 * dict.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

typedef struct {
  char *key;
  char *value;
  DictNode *next;
} DictNode;

typedef struct {
  int size;
  int mask;
  int used;
  DictNode **table;
  int (*hash_function)(const char *key);
} Dict;

Dict *dict_init();
void dict_reset(Dict *dict);

void dict_resize(Dict *dict, const unsigned int size);
DictNode *dict_find(Dict *dict, const char *key);

char *dict_get(Dict *dict, const char *key);
int dict_set(Dict *dict, const char *key, const char *value);
int dict_delete(Dict *dict, const char *key);
