# corgidb

Corgi DB is a key-value database. The project is under development.

## Getting Start

```c
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

  db_set(db, "greeting", "hello, world");

  result = db_get(db, "greeting", buff);
  if (result) {
    printf("%s\n", result);
  }

  exit(0);
}
```

## APIs

```c
// get default configuration
CorgiDBConfig *get_default_config();

// initialize database
CorgiDB *db_init(CorgiDBConfig *config);

// set command
int db_set(CorgiDB *db, const char *key, const char *value);

// set value if key was exists
int db_set_ex(const CorgiDB *db, const char *key, const char *value);

// set value if key was not exists
int db_set_nx(const CorgiDB *db, const char *key, const char *value);

// get command
char *db_get(CorgiDB *db, const char *key, char *buf);

// delete command
int db_delete(CorgiDB *db, const char *key);

// value length command
int db_strlen(CorgiDB *db, const char *key);
```

## License

Corgi DB was published under MIT license.
