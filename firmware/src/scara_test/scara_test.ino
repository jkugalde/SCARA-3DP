#include "scaraArm.h"

String msg="";

scaraArm * arm;

scaraJoint * j1;
scaraJoint * j2;
scaraJoint * j3;

//lengths

const double j1l = 186.0;
const double j2l = 164.0;
const double j3l = 83.0;
const double minr = 135.0;

//angle constraints

const double amin = 45;
const double bmin = -135.0;
const double amax = 315.0;
const double bmax = 135;

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

//gripper

const int EMpin = 50;

//instructs

String instructs[15] = {"G1 X0 Y300","G1 Z-33","M07","G1 Z0","G1 X-200 Y200","G1 Z-33","M09","G1 Z0","G1 X0 Y250","G1 Z-34","M07","G1 Z0","G1 X-300 Y100","G1 Z-35","M09"};
String buff[3];

int ninst = 3;
int nsets = 5;
int counter = 0;
int line = ninst;

void setup() {
  
  arm = new scaraArm(j1l,j2l,j3l,minr);
  j1 = new scaraJoint(3.75,true,1,-88,500.0,2000.0,4);
  j2 = new scaraJoint(3.75,true,-1,154,500.0,2000.0,4);
  j3 = new scaraJoint(5.6,false,1,0,500.0,2000.0,4);
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
  arm->setminmaxangles(amin,amax,bmin,bmax);
  arm->createGripper(EMpin,15.0,14.0,30.0);

  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  Serial.begin(115200);

  arm->homeAxes();
  arm->run();
  delay(1000);
  arm->setposangle(90.0,0.0);

}

void loop() {

  for(int i=0;i<nsets;i++){

  for(int j=0;j<ninst;j++){

   
  }
    
  }

if(arm->isReady()){

//if(msg=="G28"){
// arm->setposangle(90.0,0.0);
//}
//
//if(Serial.available()>0){
//msg=Serial.readString();  
//arm->gcode(msg);  
//}

  if(line==ninst && counter<nsets){
  buff[0]=instructs[counter*3];
  buff[1]=instructs[counter*3+1];
  buff[2]=instructs[counter*3+2];
  line=0;
  counter=counter+1;
  
  }
  else if(counter<=nsets && line<ninst){
  arm->gcode(buff[line]);
  line=line+1;
  }

  arm->run();
}
}
