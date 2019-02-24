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

  db_set(db, "key_1", "value_2", SetFlag_NONE, 0);

  db_set_range(db, "key_1", "001", 6);

  result = db_get(db, "key_1");
  if (result && result->code == RESULT_OK && result->len > 0) {
    printf("%s\n", result->buf[0]);

    free(result);
  }

  exit(0);
}