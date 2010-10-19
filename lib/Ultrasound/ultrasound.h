#pragma once
#include <stdint.h>
#include <WProgram.h>

#ifndef ULTRASOUND_MAX
#define ULTRASOUND_MAX 2
#endif

class UltraSound
{
public:
  UltraSound (int pin);
  
  static const unsigned int endless = 65535;
  static const uint8_t max = ULTRASOUND_MAX;
  
  unsigned int getRawDistance()
  {
    return distance[index];
  }
  
  
  /**
   * gets the distance in mm or @ref endless, if it is more than 3m (=3000mm) away 
   */
  unsigned int getDistanceMm ()
  {
    //TODO: update these values to the right ones
    return (distance[index] - 4000) / 5;
  }
  
  /**
   * makes a messurement and updates all the distances
   */
  static void messure ();

private:
  uint8_t index;
  static uint8_t pin[max];
  static unsigned int distance[max];
  static uint8_t numberOfUltraSound;
};