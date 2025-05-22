#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

struct SensorReadings
{
  bool irMissed;
  bool triggeredTCRT;
  bool piezoHit; // pass limit
  int  piezoValue; // raw reading
};

void initSensors();
void reset();
SensorReadings poll();

#endif
