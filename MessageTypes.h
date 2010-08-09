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

namespace Message
{

const char DATA_FROM_SOLARBOAT = 'd';
const char BATTERY = 'b';
const char REQUEST_BATTERY = 'B';
const char POTI_DATA = 'P';
const char CHANGE_MPPT_TYPE = 'M';
const char REQUEST_MPPT = 'm';
const char SEND_MPPT = '1';
const char REQUEST_MPPT_INTERVAL = 'i';
const char RESPONSE_MPPT_INTERVAL = 'I';
const char SET_MPPT_INTERVAL = '8';

// MPPT types
const char MPPT_NOMPPT = 'n';
const char MPPT_PERTURBEANDOBSERVE = 'p';
const char MPPT_ESTIMATEPERTURB = 'e';
const char MPPT_ESTIMATEESTIMATEPERTURB = 'E';
const char MPPT_UNKNOWN = 'u';



};