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
#include "pot_value.h"

namespace Fernbedienung{
namespace Pot{

int POT_MIN[2];
int POT_MAX[2];

};
};

uint8_t Fernbedienung::Pot::getMappedPotValue(Fernbedienung::Pot::Poti poti)
{
  uint8_t result;
  int value = pot_value (poti);
  if (value > max_pot (poti))
    result = 255;
  else if (value < min_pot(poti))
    result = 0;
  else
    result = (long)(value-min_pot(poti))*256/(max_pot(poti)-min_pot(poti));
  if (poti == SPEED)
    result = 255 - result;
  return result;
}


void Fernbedienung::Pot::initialize()
{
  POT_MIN[SPEED] = 397;
  POT_MAX[SPEED] = 617;
  POT_MIN[TURN] = 307;
  POT_MAX[TURN] = 731;
}
