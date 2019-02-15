/*
 * transaction.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

typedef struct Change {
  char *value;
  void *node;
  short is_new;
  struct Change *next;
} Change;

typedef struct DictTransaction {
  int began;
  Change *changes;
} DictTransaction;

int trans_begin(void *dict);
int trans_commit(void *dict);
int trans_rollback(void *dict);
int trans_add_change(void *dict_p, void *node_p, short is_new);

#endif
