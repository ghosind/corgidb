#ifndef _CORGI_CONFIG_H

typedef struct CorgiConfig {
  unsigned int (*hash_func)(const char *);
} CorgiConfig;

#endif // _CORGI_CONFIG_H
