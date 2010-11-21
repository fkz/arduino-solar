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
#include "counter.h"
#include "Dispatcher.h"

#ifndef HARDWARE_COUNTER

namespace Counter{
  
volatile unsigned long int count;

void incrementCount ();

};

void Counter::initialize()
{
  count = 0;
  pinMode (2, INPUT);
  attachInterrupt(0, &incrementCount, FALLING);
}

void Counter::incrementCount()
{
  ++count;
}



long unsigned int Counter::getCount()
{
  return count;
}

#else

namespace Counter{
  
volatile unsigned int counter;
  
};

void Counter::initialize()
{
  counter = 0;
  
  uint8_t oldSREG = SREG;
  cli();
  
  TCCR2A = 0;
  
  ASSR = (1 << EXCLK);
  
  TIMSK2 = 1 << TOIE2;
  
}

long unsigned int Counter::getCount()
{
  return ((long unsigned int)counter << 8) | TCNT0;
}

ISR(TIMER2_COMPA_vect) {
  ++counter;
}

#endif