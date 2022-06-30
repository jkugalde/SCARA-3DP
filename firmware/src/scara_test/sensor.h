#ifndef sensor_h
#define sensor_h

#include "Arduino.h"

class Sensor{
  private:
    int pin;
  public:
    Sensor(int new_pin);
    /**
     * @brief Gets the status of the sensor
     * 
     * @return true if it is activated
     * @return false if it is not activated
     */
    bool read();
};

#endif