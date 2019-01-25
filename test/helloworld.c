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

  db_set(db, "greeting", "hello, world");

  result = db_get(db, "greeting", buff);
  if (result) {
    printf("%s\n", result);
  }

  exit(0);
}