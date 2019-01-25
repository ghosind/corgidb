# corgidb

A key-value database.

## Getting Start

```c
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
```

## APIs

```c
// initialize database
CorgiDB *db_init();

// set command
int db_set(CorgiDB *db, const char *key, const char *value);

// get command
char *db_get(CorgiDB *db, const char *key, char *buf);

// delete command
int db_delete(CorgiDB *db, const char *key);

// value length command
int db_strlen(CorgiDB *db, const char *key);
```

## License

Corgi DB was published under MIT license.
