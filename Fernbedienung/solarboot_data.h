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
#include <stdint.h>

namespace Fernbedienung{
namespace SolData{

  extern char mppt;
  extern uint8_t mppt_diff;
  extern uint8_t mppt_interval;
  extern unsigned int solar_battery;
  
  
  inline void setActualMPPTType(char arg1)
  {
    mppt = arg1;
  }

  inline char getActualMPPTType ()
  {
    return mppt;
  }

  inline void setMPPTDiff(uint8_t arg1)
  {
    mppt_diff = arg1;
  }
  inline uint8_t getMPPTDiff ()
  {
    return mppt_diff;
  }


  inline void setMPPTInterval(unsigned int arg1)
  {
    mppt_interval = arg1;
  }

  inline unsigned int getMPPTInterval()
  {
    return mppt_interval;
  }
  
  inline void setSolarBattery (int value)
  {
    solar_battery = value;
  }
  
  inline int getSolarBattery ()
  {
    return solar_battery;
  }
  
  
  inline void requestMPPTDiff ()
  {
    mppt_diff = 0;
  }
  
  inline void requestMPPTInterval ()
  {
    mppt_interval = 0;
  }
  
  inline void requestSolarBattery ()
  {
    solar_battery = 0;
  }
  
  
};
};