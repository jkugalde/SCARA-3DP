#include "scaraJoint.h"


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

int nmoves = 12;
int currentmove = 0;
int movelistj1[] = {-10,0,0,0,20,0,-30,0,20,0,-20,30};
int movelistj2[] = {-10,0,0,0,20,0,-60,0,30,0,-30,30};
int movelistj3[] = {0,-43,0,43,0,0,-30,30,0,-43,43,0};
int movelistEM[] = {0,0,1,1,1,0,0,0,0,0,0,0};

void setup() {
  
  pinMode(EMpin,OUTPUT);
  digitalWrite(EMpin,LOW);

  j1 = new scaraJoint(3.75,true,1,750,5.0);
  j2 = new scaraJoint(3.75,false,1,-900,5.0);
  j3 = new scaraJoint(3.75,false,1,-200,5.0);
  j1->create_motor(stepPin1,dirPin1);
  j2->create_motor(stepPin2,dirPin2);
  j3->create_motor(stepPin3,dirPin3);
  j1->create_sensor(hom_1);
  j2->create_sensor(hom_2);
  j3->create_sensor(hom_3);
  j1->motor_setup();
  j2->motor_setup();
  j3->motor_setup();

  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  Serial.begin(9600);
  j1->add_angle(9999);
  j2->add_angle(9999);
  j3->add_angle(9999);
  j1->home();
  j2->home();
  j3->home();
  delay(5000);
  
}

void loop() {

  if(j1->isReady()*j2->isReady()*j3->isReady() == true && currentmove<nmoves){
    j1->add_angle(movelistj1[currentmove]);
    j2->add_angle(movelistj2[currentmove]);
    j3->add_angle(movelistj3[currentmove]);
    digitalWrite(EMpin,movelistEM[currentmove]);
    currentmove=currentmove+1;
  }

  if(currentmove==nmoves){
  delay(1000);
  j1->home();
  j2->home();
  j3->home();
  }

  if (j1->isReady()==false) {    
      j1->run(); 
  }

  if (j2->isReady()==false) { 
     j2->run();
  }

   if (j3->isReady()==false) { 
     j3->run();
  }
}
