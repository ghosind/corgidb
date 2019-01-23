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

  set(db, "greeting", "hello, world");

  result = get(db, "greeting", buff);
  if (result) {
    printf("%s\n", result);
  }

  exit(0);
}