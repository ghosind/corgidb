#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

  db_set(db, "key_1", "value_1", SetFlag_NONE, 0);
  db_set(db, "key_2", "value_2", SetFlag_NONE, 0);
  db_set(db, "key_3", "value_3", SetFlag_NONE, 0);
  db_set(db, "key_4", "value_4", SetFlag_NONE, 0);
  db_set(db, "key_5", "value_5", SetFlag_NONE, 0);

  result = db_keys(db);
  if (result && result->code == RESULT_OK && result->len > 0) {
    for (int i = 0; i < result->len; i++) {
      printf("%s\n", result->buf[i]);
    }
  }

  exit(0);
}
