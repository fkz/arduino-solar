#include "flags.h"

namespace Fernbedienung{ 
namespace Flags{
Flags flags = NONE;
};};

void Fernbedienung::Flags::setFlag(Fernbedienung::Flags::Flags flag, bool set)
{
  if (set)
    flags = (Flags)(flags | flag);
  else
    flags = (Flags)((~flag) & flags);
}
