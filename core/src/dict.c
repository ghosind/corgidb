#include <stdlib.h>
#include <string.h>

#include <config.h>
#include <dict.h>

CorgiDict *corgi_dict_new(CorgiConfig *config) {
  if (!config) {
    return NULL;
  }

  CorgiDict *dict = (CorgiDict *) malloc(sizeof(CorgiDict));
  if (!dict) {
    return NULL;
  }

  dict->capacity = 0;
  dict->size = 0;
  dict->data = NULL;
  dict->hash_func = config->hash_func;

  return dict;
}

void corgi_dict_free(CorgiDict *dict) {
  if (dict->data) {
    int freed = 0;
    CorgiDictNode *node, *next;
    for (int i = 0; i < dict->capacity && freed < dict->size; i++) {
      if (!dict->data[i]) {
        continue;
      }

      node = next = dict->data[i];
      while (node) {
        next = node->next;
        // TODO: replace c string to custom string.
        free(node->key);
        free(node->value);
        free(node);
        freed++;
        node = next;
      }
    }
  }

  free(dict);

  return;
}

CorgiDictNode *corgi_dict_find(CorgiDict *dict, const char *key) {
  if (!dict || !key) {
    // TODO: set errno.
    return NULL;
  }

  if (dict->size == 0 || !dict->data) {
    return NULL;
  }

  int hash = corgi_get_hash(dict, key);
  CorgiDictNode *node = dict->data[hash];
  if (!node) {
    return NULL;
  }

  while (node) {
    if (!strcmp(node->key, key)) {
      return node;
    }

    node = node->next;
  }

  return NULL;
}

char *corgi_dict_get(CorgiDict *dict, const char *key) {
  CorgiDictNode *node = corgi_dict_find(dict, key);
  if (!node) {
    return NULL;
  }

  return node->value;
}
