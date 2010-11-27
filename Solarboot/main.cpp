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
#include <WProgram.h>
#include "solarboot.h"


extern "C" void __cxa_pure_virtual () { for (;;); }

//verify, that sizeof(int)=2. If this is not the case
// this will lead to a compile time error
int COMPILE_TIME_ASSERT_INT[(sizeof(int)==2)*2-1];

int main ()
{
  init ();
  Serial.begin (MyXBee::baudRate);
  
  Solarboot::initialize();
  
  while (true)
  {
    Solarboot::dispatcher.call();
    Solarboot::xbee.readPackages();
  } 
}