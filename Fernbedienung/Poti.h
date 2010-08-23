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
#include <WProgram.h>

//TODO: think about, if a template is good (in case of size of output improvements, instance variables could be better)
template< uint8_t analog_port, unsigned int low, unsigned int high >
class Poti
{
  public:
    enum State
    {
      DOWN,
      MIDDLE,
      UP
    };
    Poti () : state (MIDDLE) { }
    void control ()
    {
      unsigned int value = analogRead (analog_port);
      State command;
      if (value < low)
	command = DOWN;
      else if (value > high)
	command = UP;
      else
	command = MIDDLE;
      if (command != state)
      {
	toRead = state = command;
      }
      else
	toRead = MIDDLE;
    }
    
    bool isDown ()
    {
      return toRead == DOWN;
    }
    
    bool isUp ()
    {
      return toRead == UP;
    }
    
  private:
    State state;
    State toRead;
};

template< uint8_t digital_port >
class PushButton
{
  public:
    PushButton () : state (false)
    {
      pinMode (digital_port, INPUT);
    }
    
    bool isPressed ()
    {
      bool actState = digitalRead (digital_port);
      if (actState != state)
      {
	state = actState;
	return state;
      }
      else
	return false;
    }
    
  private:
    bool state;
  
};