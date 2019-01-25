#include <stdio.h>
#include <stdlib.h>

#include <corgi.h>

int main(int argc, char **argv) {
  CorgiDB *db;
  char *result;
  char buff[80];

  db = db_init();
  if (!db) {
    exit(1);
  }

  db_set(db, "key_1", "value_1");
  db_set(db, "key_2", "value_2");

  db_delete(db, "key_1");

  result = db_get(db, "key_1", buff);
  if (result) {
    printf("key_1: %s\n", result);
  }

  result = db_get(db, "key_2", buff);
  if (result) {
    printf("key_2: %s\n", result);
  }

  exit(0);
}