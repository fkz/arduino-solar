#include "solarboot_data.h"
#include "MessageTypes.h"

char Fernbedienung::SolData::mppt (Message::MPPT::UNKNOWN);
uint8_t Fernbedienung::SolData::mppt_diff = 255;
uint8_t Fernbedienung::SolData::mppt_interval = 255;
unsigned int Fernbedienung::SolData::solar_battery = 65535;
