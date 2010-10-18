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