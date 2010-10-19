#include "ultrasound.h"

uint8_t UltraSound::pin[max];
unsigned int UltraSound::distance[max];
uint8_t UltraSound::numberOfUltraSound = 0;

UltraSound::UltraSound(int tpin)
{
  pin[numberOfUltraSound] = tpin;
  distance[numberOfUltraSound] = endless;
  index = numberOfUltraSound++;
}

void UltraSound::messure()
{
  for (int i = 0; i != numberOfUltraSound; ++i)
  {
    pinMode (i, OUTPUT);
    digitalWrite (i, HIGH);
  }
  delayMicroseconds(5);
  for (int i = 0; i != numberOfUltraSound; ++i)
  {
    digitalWrite (i, LOW);
    pinMode (i, INPUT);
  }
  unsigned long int starting[max];
  unsigned long int ending[max];
  for (int i = 0; i != numberOfUltraSound; ++i)
  {
    starting[i] = 0;
    ending[i] = 0;
  }
  bool running = true;
  while (running)
  {
    running = false;
    for (int i = 0; i != numberOfUltraSound; ++i)
    {
      if (starting[i] == 0)
      {
	if (digitalRead (pin[i]))
	{
	  starting[i] = micros();
	}
	else
	  running = true;
      }
    }
  }
  running = true;
  while (running)
  {
    running = false;
    for (int i = 0; i != numberOfUltraSound; ++i)
    {
      if (ending[i] == 0)
      {
	if (!digitalRead (pin[i]))
	  ending[i] = micros();
	else
	  running = true;
      }
    }
  }
  for (int i = 0; i != numberOfUltraSound; ++i)
    distance[i] = ending[i] - starting[i];
}