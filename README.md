# corgidb

![Travis CI](https://travis-ci.org/ghosind/corgidb.svg?branch=master)

Corgi DB is a key-value database. The project is under development.

## Getting Start

```c
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

  db_set(db, "greeting", "hello, world", SetFlag_NONE, 0);

  result = db_get(db, "greeting");
  if (result && result->code == RESULT_OK && result->len > 0) {
    printf("%s\n", result->buf[0]);

    free(result);
  }

  exit(0);
}
```

## APIs

```c
CorgiDBConfig *db_get_config();
CorgiDB *db_init(CorgiDBConfig *config);
CorgiDBResult *db_resize(CorgiDB *db, const unsigned int size);
CorgiDBResult *db_keys(const CorgiDB *db);
CorgiDBResult *db_set(const CorgiDB *db, const char *key, const char *value, 
    const enum DBSetFlag flag, const long ttl);
CorgiDBResult *db_set_ex(const CorgiDB *db, const char *key, const char *value, 
    const long ttl);
CorgiDBResult *db_set_nx(const CorgiDB *db, const char *key, const char *value);
CorgiDBResult *db_get(const CorgiDB *db, const char *key);
CorgiDBResult *db_get_range(const CorgiDB *db, const char *key, 
  const unsigned int start, const unsigned int end);
CorgiDBResult *db_getset(const CorgiDB *db, const char *key, const char *value);
CorgiDBResult *db_set_range(const CorgiDB *db, const char *key, const char *value, 
    const int offset);
CorgiDBResult *db_mset(const CorgiDB *db, const char ***kv_pairs, const int len, 
    const enum DBSetFlag flag, const long ttl);
CorgiDBResult *db_mset_ex(const CorgiDB *db, const char ***kv_pairs, const int len, 
    const long ttl);
CorgiDBResult *db_mset_nx(const CorgiDB *db, const char ***kv_pairs, const int len);
CorgiDBResult *db_mget(const CorgiDB *db, const char **keys, const int len);
CorgiDBResult *db_delete(const CorgiDB *db, const char *key);
CorgiDBResult *db_strlen(const CorgiDB *db, const char *key);
CorgiDBResult *db_exists(const CorgiDB *db, const char **keys, const int len);
CorgiDBResult *db_append(const CorgiDB *db, const char *key, const char *value);
CorgiDBResult *db_ttl(const CorgiDB *db, const char *key);
CorgiDBResult *db_expire(const CorgiDB *db, const char *key, const long ttl);
CorgiDBResult *db_persist(const CorgiDB *db, const char *key);
CorgiDBResult *db_flush(const CorgiDB *db);
CorgiDBResult *db_begin(const CorgiDB *db);
CorgiDBResult *db_commit(const CorgiDB *db);
CorgiDBResult *db_rollback(const CorgiDB *db);
```

## License

Corgi DB was published under MIT license.
