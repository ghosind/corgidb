#include <stdio.h>
#include <stdlib.h>

#include <corgi.h>

int main(int argc, char **argv) {
  CorgiDB *db;
  char *result;
  char buff[80];

  db = init();
  if (!db) {
    exit(1);
  }

  set(db, "key_1", "value_1");
  set(db, "key_2", "value_2");

  del(db, "key_1");

  result = get(db, "key_1", buff);
  if (result) {
    printf("key_1: %s\n", result);
  }

  result = get(db, "key_2", buff);
  if (result) {
    printf("key_2: %s\n", result);
  }

  exit(0);
}