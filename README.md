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
```

## APIs

```c
// initialize database
CorgiDB *init();

// set command
int set(CorgiDB *db, const char *key, const char *value);

// get command
char *get(CorgiDB *db, const char *key, char *buf);

// delete command
int del(CorgiDb *db, const char *key);
```

## License

Corgi DB was published under MIT license.
