# corgidb

A key-value database.

## Getting Start

```c
#include <corgi.h>

int main(int argc, char **argv) {
  CorgiDB *db;
  char *result;

  db = init();
  if (!db) {
    exit(1);
  }

  set(db, "greeting", "hello, world");
  result = get(db, "greeting");

  printf("%s\n", result);
  exit(0);
}
```

## APIs

```c
// initialize database
CorgiDB *init();

// set command
int set(CorgiDB *db, const char *key, const char *value);

// get command
char *get(CorgiDB *db, const char *key);

// delete command
int del(CorgiDb *db, const char *key);
```

## License

Corgi DB was published under MIT license.
