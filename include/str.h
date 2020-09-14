#include <stdio.h>

#ifndef _CDB_STR_H_
#define _CDB_STR_H_

#define STR_MAX_CAPACITY 0x20000000

#define STR_INIT_CAPACITY 0x10

#define str_get_head(str) ((DBStringHead *) (str - sizeof(DBStringHead)))

#define str_get_char_seq(head) ((DBString) (head + 1))

typedef char *DBString;

typedef struct DBStringHead {
  size_t length;
  size_t capacity;
} DBStringHead;

/**
 * Creates and returns a new empty string with default capacity.
 */
DBString str_create();

/**
 * Creates and returns a new string that's value copied from specified
 * characters sequence.
 */
DBString str_create_from_seq(const char *seq);

char *str_duplicate(DBString str);

size_t str_len(DBString str);

#endif // _CDB_STR_H_
