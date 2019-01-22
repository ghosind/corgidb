/*
 * dict.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <stdlib.h>
#include <string.h>

#include <dict.h>
#include <error.h>

void dict_reset(Dict* dict) {
  if (!dict) {
    db_error(1, "Cannot initialize empty dictionary.");
    return;
  }

  dict->size = 0;
  dict->mask = 0;
  dict->used = 0;
  dict->table = NULL;
}

Dict *dict_init() {
  Dict* dict;

  dict = (Dict *) malloc(sizeof(Dict));
  if (!dict) {
    db_error(1, "Failed to allocate memory for dictionary.");
    return NULL;
  }

  dict_reset(dict);

  return dict;
}

int dict_resize(Dict *dict, const int size) {
  if (!dict) {
    db_error(1, "Cannot resize empty dictionary.");
    return 1;
  }

  if (size <= 0) {
    db_error(1, "Dictionary size cannot be 0.");
    return 1;
  }

  DictNode **new_table;
  int used = dict->used;

  new_table = (DictNode **) malloc(sizeof(DictNode) * size);
  if (!new_table) {
    db_error(1, "Faled to resize dictionary.");
  }

  memset(new_table, 0, sizeof(DictNode) * size);

  for (int i = 0; i < dict->size && used > 0; i++) {
    DictNode *node = dict->table[i];

    while (node) {
      int hash_key = dict->hash_function(node->key) % dict->mask;
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
  int hash_key = dict->hash_function(key) % dict->mask;
  DictNode *node = dict->table[hash_key];

  while (node) {
    if (strcmp(node->key, key)) {
      break;
    }

    node = node->next;
  }

  return node;
}

char *dict_get(Dict *dict, const char *key) {
  if (!dict || !key) {
    db_error(1, "Dictionary and key cannot be empty");
    return NULL;
  }

  DictNode *node = dict_find(dict, key);

  return node ? node->key : NULL;
}

int dict_set(Dict *dict, const char *key, const char *value) {
  if (!dict || !key || !value) {
    db_error(1, "Dictionary, key, and value cannot be empty");
    return 1;
  }

  DictNode *node = dict_find(dict, key);

  if (node) {
    node->value = value;
    return 0;
  }

  if (dict->used == dict->size) {
    dict_resize(dict, dict->size * 2);
  }

  int hash_key = dict->hash_function(key);
  DictNode *new_node = (DictNode *) malloc(sizeof(DictNode));

  new_node->key = key;
  new_node->value = value;
  new_node->next = dict->table[hash_key];
  dict->table[hash_key] = new_node;

  return 0;
}

int dict_delete(Dict *dict, const char *key) {
  if (!dict || !key) {
    db_error(1, "Dictionary and key cannot be empty");
    return NULL;
  }
  
  int hash_key = dict->hash_function(key) % dict->mask;
  DictNode *node, *prev_node;
  
  prev_node = node = dict->table[hash_key];

  while (node) {
    if (strcmp(node->key, key)) {
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

  free(node);

  return 0;
}
