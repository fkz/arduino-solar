#pragma once

class Random
{
  private:
    long unsigned int ctx;
  public:
    static const long max;
    Random ();
    long getRandom();
    long getRandom (long max)
    {
      return getRandom() % max;
    }
};