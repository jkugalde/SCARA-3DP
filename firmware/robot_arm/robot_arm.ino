#include "AccelStepper.h" 

AccelStepper stepper_A(AccelStepper::DRIVER, 2, 5); 
AccelStepper stepper_B(AccelStepper::DRIVER, 3, 6);     

#define home_switch_A 9 
#define home_switch_B 10 
uint8_t fspeed = 4000; 

long TravelA;  
int move_finished=1;  
long initial_homing=-1; 
long TravelB;  
int move_finished_B=1;  
long initial_homing_B=-1;  

void setup() {
   Serial.begin(9600);    
   pinMode(home_switch_A, INPUT_PULLUP);
   pinMode(home_switch_B, INPUT_PULLUP);
   delay(5); 

  stepper_A.setMaxSpeed(fspeed);      
  stepper_A.setAcceleration(3000.0);  
  stepper_B.setMaxSpeed(fspeed);      
  stepper_B.setAcceleration(3000.0);  
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
 
  Serial.print("Stepper is Homing . . . . . . . . . . . ");

  while (digitalRead(home_switch_A)) { 
    stepper_A.moveTo(initial_homing);  
    initial_homing--;  
    stepper_A.run(); 
    delay(5);
  }

  stepper_A.setCurrentPosition(0); 
  stepper_A.setMaxSpeed(fspeed);      
  stepper_A.setAcceleration(3000.0);  
  initial_homing=1;

  while (!digitalRead(home_switch_A)) { 
    stepper_A.moveTo(initial_homing);  
    stepper_A.run();
    initial_homing++;
    delay(5);
  }
  
  stepper_A.setCurrentPosition(0);
  Serial.println("A Homing Completed");
  Serial.println("");
  stepper_A.setMaxSpeed(2*fspeed);      
  stepper_A.setAcceleration(3000.0);  

  while (digitalRead(home_switch_B)) { 
    stepper_B.moveTo(initial_homing);  
    initial_homing--;  
    stepper_B.run(); 
    delay(5);
  }

  stepper_B.setCurrentPosition(0);  
  stepper_B.setMaxSpeed(fspeed);      
  stepper_B.setAcceleration(3000.0);  
  initial_homing=1;

  while (!digitalRead(home_switch_B)){ 
    stepper_B.moveTo(initial_homing);  
    stepper_B.run();
    initial_homing++;
    delay(5);
  }
  
  stepper_B.setCurrentPosition(0);
  Serial.println("B Homing Completed");
  Serial.println("");
  stepper_B.setMaxSpeed(2*fspeed);      
  stepper_B.setAcceleration(3000.0);  

  Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
}

void loop() {

 while (Serial.available()>0)  { 

  String message=Serial.readString();
  int commapos = message.indexOf(',');
  String angle_A = message.substring(0,commapos);
  String angle_B = message.substring(commapos+1);
  TravelA=angle_A.toInt();
  TravelB=angle_B.toInt();
  TravelA=long(TravelA*35.555);
  TravelB=long(TravelB*35.555); 
  stepper_A.setSpeed(fspeed*2);
  stepper_B.setSpeed(fspeed*2);  
  move_finished=0;  
  
    Serial.print("Moving stepper into position A: ");
    Serial.println(TravelA);

    Serial.print("Moving stepper into position B: ");
    Serial.println(TravelB);
  
  stepper_A.moveTo(TravelA);
  stepper_B.moveTo(TravelB); 
  delay(1000);  
  }



  if ((stepper_A.distanceToGo() != 0)) {
    
    stepper_A.run(); 
    
  }

 if ((stepper_B.distanceToGo() != 0)) {
    
    stepper_B.run(); 
    
  }

  if ((move_finished == 0) && (stepper_A.distanceToGo() == 0)) {
    Serial.println("COMPLETED!");
    Serial.println("");
     Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
    move_finished=1;  
  }

   if ((move_finished == 0) && (stepper_B.distanceToGo() == 0)) {
    Serial.println("COMPLETED!");
    Serial.println("");
     Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
    move_finished=1;  
  }
  
}
