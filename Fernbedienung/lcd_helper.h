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

#include <LiquidCrystal/LiquidCrystal.h>
namespace Fernbedienung{
namespace LcdHelper{

extern LiquidCrystal lcd;
  
void writeCommaNumber(unsigned long int arg1, const char *str);
inline void writeSpannung15(int arg1)
{
  unsigned long spannung = (unsigned long)arg1 * 15271 / 1000;
  writeCommaNumber(spannung, "V");
}


  
};
};