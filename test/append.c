#include <stdio.h>
#include <stdlib.h>

#include <corgi.h>

int main(int argc, char **argv) {
  CorgiDBConfig *config;
  CorgiDB *db;
  char *result;

  config = db_get_config();
  if (!config) {
    exit(1);
  }

  db = db_init(config);
  if (!db) {
    exit(1);
  }

  db_set(db, "greeting", "hello", SetFlag_NONE);

  db_append(db, "greeting", " world!");

  result = db_get(db, "greeting");
  if (result) {
    printf("%s\n", result);

    free(result);
  }

  exit(0);
}