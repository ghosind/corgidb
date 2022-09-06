#include <config.h>

#ifndef _CORGI_DICT_H

#define corgi_get_hash(dict, key) (dict->hash_func(key) % dict->capacity)

typedef struct CorgiDictNode {
  char *key;
  char *value;
  struct CorgiDictNode *next;
} CorgiDictNode;

typedef struct CorgiDict {
  int capacity;
  int size;
  CorgiDictNode **data;
  unsigned int (*hash_func)(const char *);
} CorgiDict;

CorgiDict *corgi_dict_new(CorgiConfig *);
void corgi_dict_free(CorgiDict *);
CorgiDictNode *corgi_dict_find(CorgiDict *, const char *);
char *corgi_dict_get(CorgiDict *, const char *);

#endif // _CORGI_DICT_H
