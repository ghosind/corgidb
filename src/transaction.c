/*
 * transaction.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <string.h>

#include <cstring.h>
#include <dict.h>
#include <memory.h>
#include <result.h>
#include <transaction.h>

int trans_begin(void *dict_p) {
  Dict *dict = (Dict *) dict_p;

  if (!dict->transaction) {
    dict->transaction = (DictTransaction *) db_malloc(sizeof(DictTransaction));
    if (!dict->transaction) {
      return ERR_MEM_ALLOC;
    }

    dict->transaction->changes = NULL;
  }

  dict->transaction->status = TS_BEGAN;
  return RESULT_OK;
}

int trans_commit(void *dict_p) {
  Dict *dict = (Dict *) dict_p;
  Change *change, *next;
  
  change = dict->transaction->changes;
  dict->transaction->status = TS_COMMITTING;

  while (change) {
    next = change->next;
    free(change->value);
    free(change);
    change = next;
  }

  dict->transaction->status = TS_NONE;
  dict->transaction->changes = NULL;

  return RESULT_OK;
}

int trans_rollback(void *dict_p) {
  Dict *dict = (Dict *) dict_p;
  Change *change, *next;
  
  dict->transaction->status = TS_ROLLBACKING;
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

  dict->transaction->status = TS_NONE;
  dict->transaction->changes = NULL;

  return RESULT_OK;
}

int trans_add_change(void *dict_p, void *node_p, short is_new) {
  Dict *dict = (Dict *) dict_p;
  DictNode *node = (DictNode *) node_p;

  Change *change = (Change *) db_malloc(sizeof(Change));
  if (!change) {
    return 1;
  }

  if (!is_new) {
    char *str = cstr_get(node->value);
    char *value = (char *) db_memcpy(str, strlen(str));
    if (!value) {
      free(change);
      return ERR_MEM_ALLOC;
    }

    change->value = value;
  } else {
    change->value = NULL;
  }
  
  change->node = node;
  change->is_new = is_new;
  change->next = dict->transaction->changes;
  dict->transaction->changes = change;

  return RESULT_OK;
}
