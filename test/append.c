#include <stdio.h>
#include <stdlib.h>

#include <corgi.h>

int main(int argc, char **argv) {
  CorgiDBConfig *config;
  CorgiDB *db;
  CorgiDBResult *result;

  config = db_get_config();
  if (!config) {
    exit(1);
  }

  db = db_init(config);
  if (!db) {
    exit(1);
  }

  db_set(db, "greeting", "hello", SetFlag_NONE, 0);

  db_append(db, "greeting", " world!");

  result = db_get(db, "greeting");
  if (result && result->code == RESULT_OK && result->len > 0) {
    printf("%s\n", result->buf[0]);

    free(result);
  }

  exit(0);
}