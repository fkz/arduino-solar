#include "pot_value.h"

namespace Fernbedienung{
namespace Pot{

int POT_MIN[2];
int POT_MAX[2];

inline int min_pot (Poti p)
{
  return POT_MIN[p];
}

inline int max_pot (Poti p)
{
  return POT_MAX[p];
}

};
};

uint8_t Fernbedienung::Pot::getMappedPotValue(Fernbedienung::Pot::Poti poti)
{
  int value = pot_value (poti);
  if (value > max_pot (poti))
    return 255;
  else if (value < min_pot(poti))
    return 0;
  else
    return (long)(value-min_pot(poti))*256/(max_pot(poti)-min_pot(poti));
}


void Fernbedienung::Pot::initialize()
{
  POT_MIN[SPEED] = 397;
  POT_MAX[SPEED] = 617;
  POT_MIN[TURN] = 307;
  POT_MAX[TURN] = 731;
}
