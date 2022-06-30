#include "Stepper.h" 

Stepper stepper_A(2, 5); // create motor
Stepper stepper_B(3, 6); 
Stepper stepper_C(4, 7);     

#define home_switch_A 9 
#define home_switch_B 10 
#define home_switch_C 12
#define rele_em A3
#define maxspeed 3000
#define maxaccel 1000

uint8_t fspeed = 3000;

int em_on = 0;
long TravelA;  
int move_finished=1;  
long initial_homing=-1; 
long TravelB;  
int move_finished_B=1;  
long initial_homing_B=-1;  
long TravelC;  
int move_finished_C=1;  
long initial_homing_C=-1;  

void setup() {
  
   Serial.begin(9600);    
   pinMode(home_switch_A, INPUT_PULLUP);
   pinMode(home_switch_B, INPUT_PULLUP);
   pinMode(home_switch_C, INPUT_PULLUP);
   delay(5); 
     
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  pinMode(rele_em,OUTPUT);
  digitalWrite(rele_em,em_on);
 
  Serial.print("Stepper is Homing . . . . . . . . . . . ");

  while (digitalRead(home_switch_A)) { 
    stepper_A.moveTo(initial_homing);  
    initial_homing--;  
    stepper_A.run(); 
    delay(5);
  }

  stepper_A.setCurrentPosition(0);      
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

  while (digitalRead(home_switch_B)) { 
    stepper_B.moveTo(initial_homing);  
    initial_homing--;  
    stepper_B.run(); 
    delay(5);
  }

  stepper_B.setCurrentPosition(0);    
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

  while (digitalRead(home_switch_C)) { 
    stepper_C.moveTo(initial_homing);  
    initial_homing--;  
    stepper_C.run(); 
    delay(5);
  }

  stepper_C.setCurrentPosition(0);    
  initial_homing=1;

  while (!digitalRead(home_switch_C)){ 
    stepper_C.moveTo(initial_homing);  
    stepper_C.run();
    initial_homing++;
    delay(5);
  }
  
  stepper_C.setCurrentPosition(0);
  Serial.println("C Homing Completed");
  Serial.println("");    

  Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
  digitalWrite(rele_em,LOW);
}

void loop() {

 while (Serial.available()>0)  { 

  String message=Serial.readString();
  int commapos = message.indexOf(',');
  String angle_A = message.substring(0,commapos);
  Serial.println(angle_A);
  message = message.substring(commapos+1);
  commapos = message.indexOf(',');
  String angle_B = message.substring(0,commapos);
  Serial.println(angle_B);
  message = message.substring(commapos+1);
  commapos = message.indexOf(',');
  String pos_z = message.substring(0,commapos);
  Serial.println(pos_z);
  String electro = message.substring(commapos+1);
  Serial.println(electro);
  TravelA=angle_A.toInt();
  TravelB=angle_B.toInt();
  TravelC=pos_z.toInt();
  em_on=electro.toInt();
  TravelA=long(TravelA*8.888);
  TravelB=long(TravelB*8.888); 
  TravelC=TravelC*2.6766;
  stepper_A.setSpeed(fspeed*2);
  stepper_B.setSpeed(fspeed*2);  
  stepper_C.setSpeed(fspeed);  
  move_finished=0;  
  
    Serial.print("Moving stepper into position A: ");
    Serial.println(TravelA);

    Serial.print("Moving stepper into position B: ");
    Serial.println(TravelB);

    Serial.print("Moving stepper into position B: ");
    Serial.println(TravelC);
  
  stepper_A.moveTo(TravelA);
  stepper_B.moveTo(TravelB); 
  stepper_C.moveTo(TravelC); 
  delay(1000);  
  }



  if ((stepper_A.distanceToGo() != 0)) {
    
    stepper_A.run(); 
    
  }

 if ((stepper_B.distanceToGo() != 0)) {
    
    stepper_B.run(); 
    
  }

   if ((stepper_C.distanceToGo() != 0)) {
    
    stepper_C.run(); 
    
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

     if ((move_finished == 0) && (stepper_C.distanceToGo() == 0)) {
    Serial.println("COMPLETED!");
    Serial.println("");
     Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
    move_finished=1;  
  }
  Serial.println(em_on);
  digitalWrite(rele_em,em_on);
  
}
