#include "flags.h"

namespace Fernbedienung::Flags{
Flags flag = NONE;
};

void Fernbedienung::Flags::setFlag(Fernbedienung::Flags::Flags flag, bool set)
{
  if (set)
    flags |= flag;
  else
    flags = (~flag) & flags;
}
