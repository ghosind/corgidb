#include <stdlib.h>
#include <string.h>

#include <str.h>

DBString str_create() {
  size_t size = sizeof(DBStringHead) + STR_INIT_CAPACITY + 1;

  DBStringHead *head = (DBStringHead *) malloc(size);
  if (!head) {
    return NULL;
  }

  DBString str = str_get_char_seq(head);
  head->capacity = STR_INIT_CAPACITY;
  head->length = 0;

  return str;
}

DBString str_create_from_seq(const char *seq) {
  size_t length = strnlen(seq, STR_MAX_CAPACITY);
  size_t capacity = length + 1;
  
  if ((length + 1) % 8 != 0) {
    capacity = length & ((!0) << 3) + 8;
  }

  DBStringHead *head = (DBStringHead *) malloc(
    sizeof(DBStringHead) + capacity
  );
  if (!head) {
    return NULL;
  }

  DBString str = str_get_char_seq(head);
  strncpy(str, seq, length + 1);

  head->capacity = capacity;
  head->length = length;

  return str;
}

char *str_duplicate(DBString str) {
  size_t length = str_len(str);
  char *dest = (char *) malloc(sizeof(char) * (length + 1));

  strncpy(dest, str, length + 1);

  return dest;
}

size_t str_len(DBString str) {
  DBStringHead *head = str_get_head(str);
  if (!head) {
    return -1;
  }

  return head->length;
}
