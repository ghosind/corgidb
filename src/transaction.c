/*
 * transaction.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <string.h>

#include <cstring.h>
#include <dict.h>
#include <memory.h>
#include <transaction.h>

int trans_begin(void *dict_p) {
  Dict *dict = (Dict *) dict_p;

  if (!dict->transaction) {
    dict->transaction = (DictTransaction *) db_malloc(sizeof(DictTransaction));
    if (!dict->transaction) {
      return 1;
    }

    dict->transaction->changes = NULL;
  }

  dict->transaction->began = 1;
  return 0;
}

int trans_commit(void *dict_p) {
  Dict *dict = (Dict *) dict_p;
  Change *change, *next;
  
  change = dict->transaction->changes;

  while (change) {
    next = change->next;
    free(change->value);
    free(change);
    change = next;
  }

  dict->transaction->began = 0;
  dict->transaction->changes = NULL;

  return 0;
}

int trans_rollback(void *dict_p) {
  Dict *dict = (Dict *) dict_p;
  Change *change, *next;
  
  change = dict->transaction->changes;

  while (change) {
    next = change->next;

    if (change->is_new) {
      // delete new node.
      dict_delete(dict, cstr_get(((DictNode *) change->node)->key));
    } else {
      // change value if it is an existed node.
      cstr_set(((DictNode *) change->node)->value, change->value);
    }

    free(change->value);
    free(change);
    change = next;
  }

  dict->transaction->began = 0;
  dict->transaction->changes = NULL;

  return 0;
}

int trans_add_change(void *dict_p, void *node_p, short is_new) {
  Dict *dict = (Dict *) dict_p;
  DictNode *node = (DictNode *) node_p;

  Change *change = (Change *) db_malloc(sizeof(Change));
  if (!change) {
    return 1;
  }

  char *str = cstr_get(node->value);
  char *value = (char *) db_memcpy(str, strlen(str));
  if (!value) {
    free(change);
    return 1;
  }

  change->value = value;
  change->node = node;
  change->is_new = is_new;
  change->next = dict->transaction->changes;
  dict->transaction->changes = change;

  return 0;
}
