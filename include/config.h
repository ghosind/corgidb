/*
 * config.h - Corgi DB
 * 
 * Copyright (C) 2019, Chen Su <ghosind@gmail.com>. All right reserved.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

enum GrowthType {
  Growth_Increment,
  Growth_Multiplation,
};

enum HashType {
  Accumulation,
};

typedef struct CorgiDBConfig {
  unsigned int init_size;
  enum GrowthType growth_type;
  enum HashType hash_type;
} CorgiDBConfig;

CorgiDBConfig *get_default_config();

#endif
