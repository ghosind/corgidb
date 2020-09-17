#include <stdbool.h>

#ifndef _DB_TYPE_H_
#define _DB_TYPE_H_

typedef void *DBValue;

typedef enum DBValueType {
  VALUETYPE_UNKNOWN,
  VALUETYPE_INTEGER,
  VALUETYPE_DOUBLE,
  VALUETYPE_STRING
} DBValueType;

#ifndef _BOOL

#define _BOOL int

#endif // _BOOL

#ifndef bool

#define bool _BOOL
#define true 1
#define false 0

#endif // bool

#endif // _DB_TYPE_H_
