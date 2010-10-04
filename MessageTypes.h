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
  
template<const char type>
struct MessageData
{
  //static_assert (false, "The type is not defined as format");
  
  static const int ParamCount = -1;
};

#define MESSAGE_DATA_1(type,param) template<> \
template<> \
struct MessageData< type > \
{ \
  const int ParamCount = 1; \
  typedef param Param1; \
};

#define MESSAGE_DATA_2(type,param1,param2) template<> \
template<> \
struct MessageData< type > \
{ \
  const int ParamCount = 2; \
  typedef param1 Param1; \
  typedef param2 Param2; \
};


namespace FromSolarboat {

const char DATA_FROM_SOLARBOAT = 'd';
const char BATTERY = 'b';
const char SEND_MPPT = '1';
const char RESPONSE_MPPT_INTERVAL = 'I';
  
};

namespace ToSolarboat {

const char REQUEST_BATTERY = 'B';
const char POTI_DATA = 'P';
const char CHANGE_MPPT_TYPE = 'M';
const char REQUEST_MPPT = 'm';
const char REQUEST_MPPT_INTERVAL = 'i';
const char SET_MPPT_INTERVAL = '8';

};


// MPPT types

namespace MPPT
{

const char NOMPPT = 0;
const char PERTURBEANDOBSERVE = 1;
const char ESTIMATEPERTURB = 2;
const char ESTIMATEESTIMATEPERTURB = 3;
const char COUNT = 4;
const char UNKNOWN = 255;

};


};