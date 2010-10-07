#pragma once

namespace Fernbedienung {
namespace Flags {
  
enum Flags
{
  NONE = 0,
  BATTERY_SOLARBOOT = 1,
  BATTERY_FERNBEDIENUNG = 2,
  CONNECTION = 4,
  RECORDING = 8,
  FLAGS_ALL = 15
};


void setFlag (Flags flag, bool set);
  
};
};