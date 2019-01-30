/*
 * config.c - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#include <config.h>
#include <memory.h>

CorgiDBConfig *get_default_config() {
  CorgiDBConfig *config;

  config = (CorgiDBConfig *) db_malloc(sizeof(CorgiDBConfig));
  if (!config) {
    return NULL;
  }

  config->init_size = 1000000;
  config->growth_type = Growth_Increment;
  config->hash_type = Accumulation;

  return config;
}
