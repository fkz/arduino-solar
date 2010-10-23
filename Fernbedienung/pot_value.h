/*
    Copyright (C) 2010 Fabian Schmitthenner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#pragma once
#include <WProgram.h>
#include "fernbedienung.h"

namespace Fernbedienung{
namespace Pot{

enum Poti
{
  SPEED = 0,
  TURN = 1
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