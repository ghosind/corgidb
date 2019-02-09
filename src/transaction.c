/*
 * transaction.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

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
}

int trans_rollback(void *dict_p) {
  Dict *dict = (Dict *) dict_p;
  Change *change, *next;
  
  change = dict->transaction->changes;

  while (change) {
    next = change->next;

    cstr_set(((DictNode *) change->node)->value, change->value);

    free(change->value);
    free(change);
    change = next;
  }

  dict->transaction->began = 0;
  dict->transaction->changes = NULL;
}

int trans_add_change(void *dict_p, void *node_p) {
  Dict *dict = (Dict *) dict_p;
  DictNode *node = (DictNode *) node_p;

  Change *change = (Change *) db_malloc(sizeof(Change));
  if (!change) {
    return 1;
  }

  char *value = (char *) db_memcpy(node->value, strlen(node->value));
  if (!value) {
    free(change);
    return 1;
  }

  change->value = value;
  change->node = node;
  change->next = dict->transaction->changes;
  dict->transaction->changes = change;

  return 0;
}
