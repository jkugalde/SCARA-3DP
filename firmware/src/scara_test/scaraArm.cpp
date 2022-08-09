#include "Arduino.h"
#include "scaraArm.h"


scaraArm::scaraArm(double j1l, double j2l){

    a_joint=(scaraJoint *) malloc(sizeof (scaraJoint *));
    b_joint=(scaraJoint *) malloc(sizeof (scaraJoint *));
    z_joint=(scaraJoint *) malloc(sizeof (scaraJoint *));

    x_pos=0;
    y_pos=0;
    z_pos=0;

    a_angle=0;
    b_angle=0;

    j1_l=j1l;
    j2_l=j2l;


}

void scaraArm::setposangle(double angle_i){

    a_joint->setposangle(angle_i);

}

String scaraArm::where(){

String position = "";

position = "X: " + String(x_pos) + "  Y: " + String(y_pos) + "  Z: " + String(z_pos);

return position;


}

void scaraArm::goTo(double xpos, double ypos, double zpos){

double j2angle=acos((xpos*xpos+ypos*ypos-j1_l*j1_l-j2_l*j2_l)/(2*j1_l*j2_l))*180/3.1456;
double j1angle=(atan(ypos/xpos)-atan((j2_l*sin(j2angle*radfactor))/(j1_l+j2_l*cos(j2angle*radfactor))))*180/3.1456;

x_pos=xpos;
y_pos=ypos;
z_pos=zpos;

//goToAngle(j1angle-a_joint->aoffset,j2angle,zpos);
goToAngle(j1angle,j2angle,zpos);


Serial.println(j1angle);
Serial.println(j2angle);

}

void scaraArm::goToAngle(double angleA=0, double angleB=0, double zdis=0){


    a_joint->add_angle(angleA);
    b_joint->add_angle(angleB);
    z_joint->add_angle(zdis);
    a_angle=angleA;
    b_angle=angleB;
    z_pos=zdis;
    matchSpeeds();



}


void scaraArm::goToAngleRel(double angleA=0, double angleB=0, double zdis=0){

    a_angle=a_angle+angleA;
    b_angle=b_angle+angleB;
    z_pos=z_pos+zdis;
    a_joint->add_angle(a_angle);
    b_joint->add_angle(b_angle);
    z_joint->add_angle(z_pos);

    matchSpeeds();

}

void scaraArm::setSpeeds(double speeda, double speedb, double speedz){

    a_joint->setMaxSpeed(speeda);
        b_joint->setMaxSpeed(speedb);
            z_joint->setMaxSpeed(speedz);

}

void scaraArm::matchSpeeds(){

    double va =   a_joint->distanceToGo();
        double vb = b_joint->distanceToGo();
            double vz = z_joint->distanceToGo();


            double r0=0;
            double r1=0;
            double r2=0;


if(va>vb){

r0=1;
r1=vb/va;
r2=vz/va;
    
}

else{

r0=va/vb;
r1=1;
r2=vz/vb;

}

setSpeeds(r0,r1,r2);

}

void scaraArm::run(){

  while(!(a_joint->isReady()*b_joint->isReady()*z_joint->isReady())){
    
    a_joint->run(); 
    b_joint->run();
    z_joint->run();


  }
}


void scaraArm::homeAxes(){

    z_joint->home();
    a_joint->home();
    b_joint->home();
    a_angle=90;
    b_angle=0;
    x_pos=0;
    y_pos=345;
    z_pos=0;

}

void scaraArm::registerJoint(scaraJoint * ajoint, scaraJoint * bjoint, scaraJoint * zjoint){

    a_joint=ajoint;
    b_joint=bjoint;
    z_joint=zjoint;   

}

void scaraArm::createGripper(int epin){
    e_pin=epin;
    e_state=0;
    pinMode(e_pin,OUTPUT);

}

bool scaraArm::isReady(){

return a_joint->isReady()*b_joint->isReady()*z_joint->isReady();

}

void scaraArm::gcode(String msg){

String sub = "";
String gtype = "";
int i = msg.indexOf(" ");

if(i==-1){
    gtype=msg;
    sub="";
}    

else{
gtype = msg.substring(0,i);
sub = msg.substring(i+1);    
}


if(gtype=="G1"){
    g1(sub);
}
if(gtype=="G28"){
    homeAxes();
}

}


void scaraArm::g1(String coord){

int xindex = coord.indexOf("X");
String xpostring = "";
int yindex = coord.indexOf("Y");
String ypostring = "";
int zindex = coord.indexOf("Z");
String zpostring = "";
int findex = coord.indexOf("F");
String speedstring = "";
double xpos = 0;
double ypos = 0;
double zpos = 0;
double fspeed = 0;

if(xindex!=-1){
    xpostring=coord.substring(xindex+1,coord.indexOf(" ",xindex));
    xpos=xpostring.toDouble();
}
if(yindex!=-1){
    ypostring=coord.substring(yindex+1,coord.indexOf(" ",yindex));
    ypos=ypostring.toDouble();
}
if(zindex!=-1){
    zpostring=coord.substring(zindex+1,coord.indexOf(" ",zindex));
    zpos=zpostring.toDouble();
}

goTo(xpos,ypos,zpos); 


}


