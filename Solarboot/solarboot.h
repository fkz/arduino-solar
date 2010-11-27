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
#include <wireless.h>
#include <LiquidCrystal/LiquidCrystal.h>
#include <Dispatcher.h>
#include <Servo/Servo.h>
#include <Ultrasound/ultrasound.h>


class MPPT;

namespace Solarboot
{
    extern NDispatcher dispatcher;
    extern MyXBee xbee;
  
    static const int motorId = 9;
    static const int turnId = 10;
    
    static const int stromId = 0;
    static const int spannungId = 1;

    static const int BATTERY = 2;
    
    static const int readStromCount = 50;
    
     /// initialize pin modes and XBee connection
    void initialize();
    void changeMPPT();
    void checkBattery ();
};