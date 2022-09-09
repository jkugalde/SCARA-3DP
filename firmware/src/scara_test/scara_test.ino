#include "scaraArm.h"

String msg="";

scaraArm * arm; // creates scara arm

scaraJoint * j0; // creates the joints, shoulder, elbow and z.
scaraJoint * j1;
scaraJoint * j2;

//lengths of the arm (mm), forearm, and z movement range until it hit the floor (without effector)

const double a0 = 186.0;
const double a1 = 164.0;
const double a2 = 83.0;

// constraints

const double q0min = 45; //shoulder angle restrictions
const double q0max = 315.0;

const double q1min = -135.0; //elbow angle restrictions
const double q1max = 135;

const double minr = 135.0; //inner task space radius limitation (mm)

// Shoulder motor

const int dirPin0 = 5;
const int stepPin0 = 2;

// Elbow motor

const int dirPin1 = 6;
const int stepPin1 = 3;

// Z motor

const int dirPin2 = 7;
const int stepPin2 = 4;

//home sensors

const int hom_1 = 9; //shoulder
const int hom_2 = 10; //elbow
const int hom_3 = 12; //z

//gripper

const int EMpin = 50;

//instructions if using pre programmed moves, in this case there are 15 moves

String instructs[15] = {"G1 X0 Y300","G1 Z-33","M07","G1 Z0","G1 X-200 Y200","G1 Z-33","M09","G1 Z0","G1 X0 Y250","G1 Z-34","M07","G1 Z0","G1 X-300 Y100","G1 Z-35","M09"};

int ninst = 15; // n instructions, 
int line = 0; // current move

void setup() {
  
  arm = new scaraArm(a0,a1,a2,minr);
  j0 = new scaraJoint(3.75,true,1,-88,500.0,2000.0,4); // (transmission multiplier, invert direction, homing direction, angle offset, maxvel, acceleration, microstepping) 
  j1 = new scaraJoint(3.75,true,-1,154,500.0,2000.0,4);
  j2 = new scaraJoint(5.6,false,1,0,4000.0,10000.0,4);
  j0->create_motor(stepPin0,dirPin0);
  j1->create_motor(stepPin1,dirPin1);
  j2->create_motor(stepPin2,dirPin2);
  j0->create_sensor(hom_1);
  j1->create_sensor(hom_2);
  j2->create_sensor(hom_3);
  j0->motor_setup();
  j1->motor_setup();
  j2->motor_setup();
  arm->registerJoint(j0,j1,j2); // includes joints int arm
  arm->setminmaxangles(q0min,q0max,q1min,q1max); //set angle restrictions
  arm->createGripper(EMpin,15.0,14.0,30.0); //actuation pin and center point offset from the end of the forearm. 

  pinMode(8,OUTPUT); //enable in CNC Shield
  digitalWrite(8,LOW);
  Serial.begin(115200);

  arm->homeAxes();
  arm->run();
  delay(1000);
  arm->setposangle(90.0,0.0); //set initial position after homing, in this case, 90 degrees more than the natural position.

}

void loop() {

if(arm->isReady()){

if(msg=="G28"){ //set initial position after homing
 arm->setposangle(90.0,0.0);
}

if(Serial.available()>0){ // code to receive G-Code in the serial monitor
msg=Serial.readString();  
arm->gcode(msg);  
}

//  if(line<ninst){ //uncomment to use the pre programmed moves and comment the serial gcode receiver.
//  msg=instructs[line];
//  arm->gcode(msg);
//  line=line+1;
//  }

  arm->run();
}
}
