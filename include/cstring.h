/*
 * cstring.h - Corgi DB
 *
 * String type for corgi db.
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#ifndef _CSTRING_H_
#define _CSTRING_H_

typedef struct CString {
  int size;
  int used;
  char *buffer;
} CString;

CString *cstr_create(const char *str);
char *cstr_get(CString *node);
int cstr_set(CString *node, const char *str);
int cstr_set_range(CString *node, const char *str, int offset);
void cstr_free(CString *node);
int cstr_len(CString *node);
int cstr_is_equal(CString *node, const char *str);
int cstr_cmp(CString *node, const char *str);
int cstr_append(CString *node, const char *str);

#endif
