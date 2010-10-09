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
#ifdef OLD_CASE
    objects[i] = 0;
#endif
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
#ifdef OLD_CASE
      if (exec[i] != 0)
	(objects[i]->*exec[i])();
#else
      if (exec[i] != 0)
	exec[i]();
#endif
    }
  }
}

template< int count >
#ifdef OLD_CASE
void Dispatcher<count>::setMethod(int id, Dispatcheable *object, void (Dispatcheable::*method) () , unsigned long interval )
#else
void Dispatcher<count>::setMethod(int id, void (*method) () , unsigned long interval )
#endif
{
  this->exec[id] = method;
  this->interval[id] = interval;
  this->lastCall[id] = 0;
#ifdef OLD_CASE
  this->objects[id] = object;
#endif
}
