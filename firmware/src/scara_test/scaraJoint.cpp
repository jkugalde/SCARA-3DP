#include "Arduino.h"
#include "scaraJoint.h"
#include "sensor.h"
#include "AccelStepper.h"

scaraJoint::scaraJoint(double a_ratio,bool a_inverted,int a_homing_dir,int a_offset,double a_speed_multiplier){

    motor =(AccelStepper *) malloc(sizeof (AccelStepper *));
    sensor = (Sensor *) malloc(sizeof(Sensor *));
    ratio = a_ratio;
    inverted = a_inverted;
    homing_dir = a_homing_dir;
    offset = a_offset;
    angle = 0;
    position =0;
    speed_multiplier = a_speed_multiplier;

}


void scaraJoint::home(){
    int times_activated = 0;
    int tolerance = 7;
    while(true){

        if (sensor->read()){
            times_activated++;

        }else{
            times_activated =0;
        }

        if (times_activated>=tolerance){
                motor->setCurrentPosition(offset*-1);
                position=0;
                angle=0;
                motor->moveTo(position);
                break;
        }
            if(motor->distanceToGo() == 0){ // Homing failed, we return to avoid soft lock.
                return;
            }
            motor->run();

    }
}


void scaraJoint::create_motor(int step_pin,int dir_pin){
    motor = new AccelStepper(1,step_pin, dir_pin);
    
 }

 void scaraJoint::motor_setup(){

     double m_speed = speed_multiplier*SPEED_CONSTANT;
         //motor->invertDirPin(inverted);
         motor->setCurrentPosition(0);
         motor->setSpeed(m_speed);
         motor->setAcceleration(8000);       
    

 }
void scaraJoint::create_sensor(int pin){
    sensor = new Sensor(pin);
 }

void scaraJoint::show(){
    Serial.print("Joint ");
    Serial.print(index);
    Serial.print(" Motor speed ");
    Serial.print(" Joint ratio: ");
    Serial.print(ratio);
    Serial.println("");

}

void scaraJoint::add_angle(long val){
    angle+=val;
    position = (long) ((ratio*angle/1.8*MICRO_STEPPING));
    Serial.println(angle);
    Serial.println(position);
        motor->moveTo(position);
    
}

void scaraJoint::launch_home(){
    add_angle(1.3*3.1415*ACC*homing_dir);

      motor->moveTo(position);
    
}

void scaraJoint::set_offset(long new_offset){
    offset = new_offset;
}
void scaraJoint::show_offset(){
    Serial.print("Joint ");
    Serial.print(index);
    Serial.print(" current offset: ");
    Serial.print(offset);
}

void scaraJoint::run(){
    motor->run();
}

long scaraJoint::distanceToGo(){
    return motor->distanceToGo();
}

bool scaraJoint::isReady(){
    if(distanceToGo()==0){
        return true;
    }
    else{
        return false;
    }
}