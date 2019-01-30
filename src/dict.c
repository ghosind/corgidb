/*
 * dict.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <stdlib.h>
#include <string.h>

#include <config.h>
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
    return 1;
  }

  if (size <= 0) {
    db_error(1, "Dictionary size cannot be 0.");
    return 1;
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
      int hash_key = get_hash(dict, node->key);
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

DictNode *dict_find(Dict *dict, const char *key) {
  if (dict->size == 0) {
    return NULL;
  }

  int hash_key = get_hash(dict, key);
  DictNode *node = dict->table[hash_key];

  while (node) {
    if (!strcmp(node->key, key)) {
      break;
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

  DictNode *node = dict_find(dict, key);

  return node ? node->value : NULL;
}

int dict_set(Dict *dict, const char *key, const char *value, const enum DBSetFlag flag) {
  if (!dict || !key || !value) {
    db_error(ERR_SYS_PARAMS, "Dictionary, key, and value cannot be empty");
    return 1;
  }

  char *value_buf = (char *) db_malloc(sizeof(char) * strlen(value));
  if (!value_buf) {
    return ERR_MEM_ALLOC;
  }
  
  strcpy(value_buf, value);

  DictNode *node = dict_find(dict, key);

  if (node) {
    if (flag == SetFlag_NX) {
      return 1;
    }

    node->value = value_buf;
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

  char *key_buf = (char *) db_malloc(sizeof(char) * strlen(key));
  if (!key_buf) {
    return ERR_MEM_ALLOC;
  }

  strcpy(key_buf, key);

  new_node->key = key_buf;
  new_node->value = value_buf;
  new_node->next = dict->table[hash_key];
  dict->table[hash_key] = new_node;

  return 0;
}

int dict_delete(Dict *dict, const char *key) {
  if (!dict || !key) {
    db_error(ERR_SYS_PARAMS, "Dictionary and key cannot be empty");
    return 1;
  }

  int hash_key = get_hash(dict, key);
  DictNode *node, *prev_node;
  
  prev_node = node = dict->table[hash_key];

  while (node) {
    if (!strcmp(node->key, key)) {
      break;
    }

    prev_node = node;
    node = node->next;
  }

  if (node == NULL) {
    // key is not exists.
    return 1;
  }

  if (prev_node == node) {
    dict->table[hash_key] = node->next;
  } else {
    prev_node->next = node->next;
  }

  free(node->key);
  free(node->value);
  free(node);

  return 0;
}
