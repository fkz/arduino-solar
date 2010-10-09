#pragma once
#include <WProgram.h>
#include "fernbedienung.h"

namespace Fernbedienung{
namespace Pot{

enum Poti
{
  SPEED,
  TURN
};


void initialize ();

inline int pot_value(Poti poti)
{
  return analogRead (poti == SPEED ? POT_SPEED : POT_TURN);
}

/** mapps the value read by analogRead which is between 0 and 1023 to a value between 0 and 255; where 128 is the middle
 * @p poti the poti (either SPEED or TURN)
 */
uint8_t getMappedPotValue (Poti poti);

extern int POT_MIN[2];
extern int POT_MAX[2];

inline int &min_pot (Poti p)
{
  return POT_MIN[p];
}

inline int &max_pot (Poti p)
{
  return POT_MAX[p];
}

extern uint8_t trim_poti;

};  
};