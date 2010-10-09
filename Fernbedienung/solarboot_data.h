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