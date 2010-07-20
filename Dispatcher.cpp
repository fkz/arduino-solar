#include <WProgram.h>
#include <inttypes.h>

template< int count >
Dispatcher<count>::Dispatcher()
: actualId (0)
{
  for (int i = 0; i != count; ++i)
  {
    exec[i] = 0;
    interval[i] = 100000;
    lastCall[i] = 0;
    objects[i] = 0;
  }
}

template< int count >
void Dispatcher<count>::call()
{
  long unsigned int now = millis();
  for (int i = 0; i != count; ++i)
  {
    if (lastCall[i] <= now)
    {
      lastCall[i] = now + interval[i];
      (objects[i]->*exec[i])();
    }
  }
}

template< int count >
void Dispatcher<count>::setMethod(int id, Dispatcheable *object, void (Dispatcheable::*method) () , unsigned long interval )
{
  this->exec[id] = method;
  this->interval[id] = interval;
  this->lastCall[id] = 0xFFFF;
  this->objects[id] = object;
}
