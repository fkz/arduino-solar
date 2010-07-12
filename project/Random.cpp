#include "Random.h"
#include <stdlib.h>

const long Random::max = RANDOM_MAX;

Random::Random()
: ctx (0x6281)
{
  
}

long Random::getRandom()
{
  return random_r(&ctx);
}