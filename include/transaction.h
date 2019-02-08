/*
 * transaction.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include <dict.h>

typedef struct Change {
  char *value;
  void *node;
  struct Change *next;
} Change;

typedef struct DictTransaction {
  int began;
  Change *changes;
} DictTransaction;

int trans_begin(void *dict);
int trans_commit(void *dict);
int trans_rollback(void *dict);

#endif