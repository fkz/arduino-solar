#pragma once
#include <WConstants.h>
#include "fernbedienung.h"

namespace Fernbedienung{
namespace Pot{

enum Poti
{
  SPEED,
  TURN
};


void initialize ();

int pot_value(Poti poti)
{
  return analogRead (poti == SPEED ? POT_SPEED : POT_TURN);
}

/** mapps the value read by analogRead which is between 0 and 1023 to a value between 0 and 255; where 128 is the middle
 * @p poti the poti (either SPEED or TURN)
 */
uint8_t getMappedPotValue (Poti poti);


uint8_t trim_poti;

};  
};