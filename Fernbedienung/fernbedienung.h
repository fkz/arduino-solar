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
#include "../wireless.h"
#include "Menu.h"
#include "Poti.h"

namespace Fernbedienung{

void initialize ();
  
extern MyXBee xbee;
extern NDispatcher dispatcher;
extern FileManagement files;
//extern Menu menu;

// Konstanten
const uint8_t LCD_RS = 12;
const uint8_t LCD_ENABLE = 11;
const uint8_t LCD_D0 = 5;
const uint8_t LCD_D1 = 4;
const uint8_t LCD_D2 = 3;
const uint8_t LCD_D3 = 2;

const uint8_t POT_SPEED = 0;
const uint8_t POT_TURN = 1;
const uint8_t BATTERY = 3;
const int MIN_BATTERY_VALUE = 570;

const uint8_t POT_STEUERUNG_X = 4;
const uint8_t POT_STEUERUNG_Y = 2;
const uint8_t STEUERUNG_PRESS = 10;


void sendData ();

namespace Buttons{

extern Poti< POT_STEUERUNG_X, 400, 640 > steuerungX;
extern Poti< POT_STEUERUNG_Y, 400, 640 > steuerungY;
extern PushButton< STEUERUNG_PRESS > steuerungPress;

void controlButtons();
};

void checkBatteryState();
void menuInterval();

extern int recordId;
};