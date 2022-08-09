#include "scaraArm.h"

String msg="";

scaraArm * arm;

scaraJoint * j1;
scaraJoint * j2;
scaraJoint * j3;

// Motor 1
const int dirPin1 = 5;
const int stepPin1 = 2;

// Motor 2
const int dirPin2 = 6;
const int stepPin2 = 3;

// Motor 3
const int dirPin3 = 7;
const int stepPin3 = 4;

//hom sensors
const int hom_1 = 9;
const int hom_2 = 10;
const int hom_3 = 12;

const int EMpin = A3;

void setup() {
  
  arm = new scaraArm(182.0,163.0);
  j1 = new scaraJoint(3.75,true,1,-89.5,500.0,2000.0,4);
  j2 = new scaraJoint(3.75,true,-1,170,500.0,2000.0,4);
  j3 = new scaraJoint(2.4,false,1,0,500.0,2000.0,4);
  j1->create_motor(stepPin1,dirPin1);
  j2->create_motor(stepPin2,dirPin2);
  j3->create_motor(stepPin3,dirPin3);
  j1->create_sensor(hom_1);
  j2->create_sensor(hom_2);
  j3->create_sensor(hom_3);
  j1->motor_setup();
  j2->motor_setup();
  j3->motor_setup();
  arm->registerJoint(j1,j2,j3);
  arm->createGripper(EMpin);

  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  Serial.begin(115200);

  arm->homeAxes();
  arm->run();
  delay(1000);
  arm->setposangle(90.0);

}

void loop() {

if(arm->isReady()){

if(Serial.available()>0){
msg=Serial.readString();  
Serial.println(msg);
arm->gcode(msg);  
arm->run();
}
 
}


}
