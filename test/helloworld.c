#include <stdio.h>
#include <stdlib.h>

#include <corgi.h>

int main(int argc, char **argv) {
  CorgiDBConfig *config;
  CorgiDB *db;
  char *result;
  char buff[80];

  config = get_default_config();
  if (!config) {
    exit(1);
  }

  db = db_init(config);
  if (!db) {
    exit(1);
  }

  db_set(db, "greeting", "hello, world", SetFlag_NONE);

  result = db_get(db, "greeting", buff);
  if (result) {
    printf("%s\n", result);
  }

  exit(0);
}