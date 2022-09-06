#include "Arduino.h"
#include "scaraJoint.h"
#include "sensor.h"
#include "AccelStepper.h"

scaraJoint::scaraJoint(double a_ratio,bool a_inverted,int a_homing_dir,double a_offset,double a_speed, double a_accel, int a_stepping){

    motor =(AccelStepper *) malloc(sizeof (AccelStepper *));
    sensor = (Sensor *) malloc(sizeof(Sensor *));
    ratio = a_ratio;
    inverted = a_inverted;
    homing_dir = a_homing_dir;
    angle = 0;
    position = 0;
    speed = a_speed;
    accel = a_accel;
    stepping = a_stepping;
    aoffset=a_offset;
    offset = (long)((ratio*a_offset/1.8*stepping));

}

void scaraJoint::setposangle(double angle_i){

    position=(long)((ratio*angle_i/1.8*stepping));
    angle=angle_i;
    motor->setCurrentPosition(position);

}


void scaraJoint::home(){

    int times_activated = 0;
    int tolerance = 7;

    add_angle(1000*homing_dir);
    //setMaxSpeed(1.0);

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

    //setMaxSpeed(1.0);

}

void scaraJoint::setMaxSpeed(double b_speed){
    motor->setMaxSpeed(speed*b_speed);

}


void scaraJoint::create_motor(int step_pin,int dir_pin){
    motor = new AccelStepper(1,step_pin, dir_pin);
    
 }

 void scaraJoint::motor_setup(){

         //motor->invertDirPin(inverted);
         motor->setCurrentPosition(0);
         motor->setSpeed(speed);
         motor->setAcceleration(accel);
         motor->setPinsInverted(inverted,0,0);    
         motor->setMaxSpeed(speed);   
    

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

void scaraJoint::add_angle(double val){
    angle=val;
    position = (long)((ratio*val/1.8*stepping));
        motor->moveTo(position);
    
}

void scaraJoint::set_offset(long new_offset){
    offset = new_offset;
}

int scaraJoint::give_offset(){

return offset;

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