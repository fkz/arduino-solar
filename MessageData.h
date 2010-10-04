#pragma once

namespace Message
{

template<const char type>
struct MessageData
{
  static const int ParamCount = -1;
};

};

#define MESSAGE_DATA_1(type,param,name) template<> \
template<> \
struct MessageData< type > \
{ \
  static const int ParamCount = 1; \
  typedef param Param1; \
  param name; \
};

#define MESSAGE_DATA_2(type,param1,name1,param2,name2) template<> \
template<> \
struct MessageData< type > \
{ \
  static const int ParamCount = 2; \
  typedef param1 Param1; \
  typedef param2 Param2; \
  param1 name1; \
  param2 name2; \
};

#define MESSAGE_DATA_3(type,param1,name1,param2,name2,param3,name3) template<> \
template<> \
struct MessageData< type > \
{ \
  static const int ParamCount = 3; \
  typedef param1 Param1; \
  typedef param2 Param2; \
  typedef param3 Param3; \
  param1 name1; \
  param2 name2; \
  param3 name3; \
};
