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
#include "MessageData.h"

namespace Message
{

namespace FromSolarboat {

const char DATA_FROM_SOLARBOAT = 'd';
const char BATTERY = 'b';
const char SEND_MPPT = '1';
const char RESPONSE_MPPT_INTERVAL = 'I';
};

MESSAGE_DATA_5(FromSolarboat::DATA_FROM_SOLARBOAT, unsigned int, strom, unsigned int, spannung, char, mpptType, unsigned long int, freqCount, uint8_t, mpptData);
MESSAGE_DATA_1(FromSolarboat::BATTERY, unsigned int, data);
MESSAGE_DATA_2(FromSolarboat::SEND_MPPT, const char, subtype, char, data);
MESSAGE_DATA_1(FromSolarboat::RESPONSE_MPPT_INTERVAL, unsigned int, interval);

namespace ToSolarboat {

const char REQUEST_BATTERY = 'B';
const char POTI_DATA = 'P';
const char CHANGE_MPPT_TYPE = 'M';
const char DATA_TO_MPPT = 'v';

};

MESSAGE_DATA_2 (ToSolarboat::POTI_DATA, uint8_t, speed, uint8_t, turn);
MESSAGE_DATA_1 (ToSolarboat::CHANGE_MPPT_TYPE, uint8_t, mpptType);
MESSAGE_DATA_1 (ToSolarboat::DATA_TO_MPPT, uint8_t, data);
MESSAGE_DATA_0 (ToSolarboat::REQUEST_BATTERY);


// MPPT types

namespace MPPT
{

const char NOMPPT = 0;
const char PERTURBEANDOBSERVE = 1;
const char ESTIMATEPERTURB = 2;
const char ESTIMATEESTIMATEPERTURB = 3;
const char CONSTMPPT = 4;
const char COUNT = 5;
const char UNKNOWN = 255;

};


};