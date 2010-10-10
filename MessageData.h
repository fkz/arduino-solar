#pragma once

namespace Message
{

template<const char type>
struct MessageData
{
  static_assert (type != type, "intantiation of not specialized MessageTypes is not allowed");
  static const int ParamCount = -1;
};

};

#define MESSAGE_DATA_1(type,param,name) \
template<> \
struct MessageData< type > \
{ \
  static const int ParamCount = 1; \
  typedef param Param1; \
  param name; \
} __attribute__((__packed__));

#define MESSAGE_DATA_2(type,param1,name1,param2,name2) \
template<> \
struct MessageData< type > \
{ \
  static const int ParamCount = 2; \
  typedef param1 Param1; \
  typedef param2 Param2; \
  param1 name1; \
  param2 name2; \
} __attribute__((__packed__));

#define MESSAGE_DATA_3(type,param1,name1,param2,name2,param3,name3) \
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
} __attribute__((__packed__));
