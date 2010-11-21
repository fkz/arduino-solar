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
bool getFlag (Flags flag);  


};
};