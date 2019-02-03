/*
 * dict.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <stdlib.h>
#include <string.h>

#include <config.h>
#include <cstring.h>
#include <dict.h>
#include <error.h>
#include <hash.h>
#include <memory.h>

void dict_reset(Dict* dict) {
  if (!dict) {
    db_error(ERR_SYS_PARAMS, "Cannot initialize empty dictionary.");
    return;
  }

  dict->size = 0;
  dict->mask = 0;
  dict->used = 0;
  dict->table = NULL;
}

Dict *dict_init(CorgiDBConfig *config) {
  Dict* dict;

  dict = (Dict *) db_malloc(sizeof(Dict));
  if (!dict) {
    return NULL;
  }

  dict_reset(dict);

  switch (config->hash_type) {
    case Accumulation:
      dict->hash_function = accumulation;
      break;
    default:
      db_error(ERR_UNKNOWN_HASH, "Unknow hash type.");
      free(dict);
      return NULL;
  }

  return dict;
}

int dict_resize(Dict *dict, const int size) {
  if (!dict) {
    db_error(ERR_SYS_PARAMS, "Cannot resize empty dictionary.");
    return ERR_SYS_PARAMS;
  }

  if (size <= 0) {
    db_error(ERR_DICT_SIZE, "Dictionary size cannot be 0.");
    return ERR_DICT_SIZE;
  }

  DictNode **new_table;
  int used = dict->used;

  new_table = (DictNode **) db_malloc(sizeof(DictNode) * size);
  if (!new_table) {
    return ERR_MEM_ALLOC;
  }

  memset(new_table, 0, sizeof(DictNode) * size);

  for (int i = 0; i < dict->size && used > 0; i++) {
    DictNode *node = dict->table[i];

    while (node) {
      int hash_key = get_hash(dict, cstr_get(node->key));
      DictNode *new = node;
      node = node->next;

      new->next = new_table[hash_key];
      new_table[hash_key] = new;
      used--;
    }
  }

  free(dict->table);

  dict->size = size;
  dict->mask = size;
  dict->table = new_table;

  return 0;
}

DictNode *dict_find(Dict *dict, const char *key, DictNode *prev) {
  if (dict->size == 0) {
    return NULL;
  }

  int hash_key = get_hash(dict, key);
  DictNode *node = dict->table[hash_key];

  while (node) {
    if (cstr_is_equal(node->key, key)) {
      break;
    }

    if (prev != NULL) {
      prev = node;
    }

    node = node->next;
  }

  return node;
}

char *dict_get(Dict *dict, const char *key) {
  if (!dict || !key) {
    db_error(ERR_SYS_PARAMS, "Dictionary and key cannot be empty");
    return NULL;
  }

  DictNode *node = dict_find(dict, key, NULL);

  return node ? cstr_get(node->value) : NULL;
}

int dict_set(Dict *dict, const char *key, const char *value, const enum DBSetFlag flag) {
  if (!dict || !key || !value) {
    db_error(ERR_SYS_PARAMS, "Dictionary, key, and value cannot be empty");
    return 1;
  }

  DictNode *node = dict_find(dict, key, NULL);

  if (node) {
    if (flag == SetFlag_NX) {
      return 1;
    }

    cstr_set(node->value, value);
    return 0;
  }

  if (flag == SetFlag_XX) {
    return 1;
  }

  if (dict->used == dict->size) {
    dict_resize(dict, dict->size > 0 ? dict->size * 2 : 100);
  }

  int hash_key = get_hash(dict, key);
  DictNode *new_node = (DictNode *) db_malloc(sizeof(DictNode));
  if (!new_node) {
    return ERR_MEM_ALLOC;
  }

  new_node->key = cstr_create(key);
  new_node->value = cstr_create(value);
  new_node->next = dict->table[hash_key];
  dict->table[hash_key] = new_node;

  return 0;
}

int dict_delete(Dict *dict, const char *key) {
  if (!dict || !key) {
    db_error(ERR_SYS_PARAMS, "Dictionary and key cannot be empty");
    return 1;
  }

  DictNode *node, *prev;
  
  // get node and previous node.
  node = dict_find(dict, key, prev);

  if (node == NULL) {
    // key is not exists.
    return 1;
  }

  int hash_key = get_hash(dict, key);
  if (prev == NULL) {
    // it is the first node of list.
    dict->table[hash_key] = node->next;
  } else {
    prev->next = node->next;
  }

  cstr_free(node->key);
  cstr_free(node->value);
  free(node);

  return 0;
}
