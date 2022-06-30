#include "Arduino.h"
#include "sensor.h"

Sensor::Sensor(int new_pin){
     pin = new_pin;
     pinMode(pin,INPUT_PULLUP);
 }


bool Sensor::read(){
    return !digitalRead(pin);
}