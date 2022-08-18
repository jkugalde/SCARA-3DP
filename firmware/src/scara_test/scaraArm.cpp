#include "Arduino.h"
#include "scaraArm.h"


scaraArm::scaraArm(double j1l, double j2l, double j3l, double minr){

    a_joint=(scaraJoint *) malloc(sizeof (scaraJoint *));
    b_joint=(scaraJoint *) malloc(sizeof (scaraJoint *));
    z_joint=(scaraJoint *) malloc(sizeof (scaraJoint *));

    x_pos=0;
    y_pos=0;
    z_pos=0;

    a_angle=0;
    b_angle=0;

    a_angle_min=0;
    a_angle_max=0;

    b_angle_min=0;
    b_angle_max=0;

    j1_l=j1l;
    j2_l=j2l;
    j3_l=j3l;

    offsetx=0;
    offsety=0;


    max_r = (j1_l+j2_l)*(j1_l+j2_l);
    min_r = minr*minr;

}

void scaraArm::setposangle(double angle_i, double angle_j){

    a_joint->setposangle(angle_i);
    b_joint->setposangle(angle_j);

}

String scaraArm::where(){

String position = "";

position = "X: " + String(x_pos) + "  Y: " + String(y_pos) + "  Z: " + String(z_pos);

return position;


}

void scaraArm::goTo(double xpos, double ypos, double zpos){

double j2angle=0;
double j1angle=0;

double mainangle = atan2(ypos,xpos);

double limitangle=atan2(ypos,xpos)*180/3.1459;

if(mainangle<0){
mainangle=360+mainangle;
}

if(xpos!=x_pos){

xpos=xpos-offsetx*sin(mainangle)-offsety*cos(mainangle);

}

if(ypos!=y_pos){

ypos=ypos-offsety*sin(mainangle)+offsetx*cos(mainangle);
}


bool limit = false;

if(limitangle>=0 && limitangle>a_angle_min){
     limit=true;
 }
if(limitangle<0 && limitangle<a_angle_max-360){
     limit=true;
}


if(xpos*xpos+ypos*ypos<=max_r && xpos*xpos+ypos*ypos >= min_r && limit==true){


j2angle=abs(acos((xpos*xpos+ypos*ypos-j1_l*j1_l-j2_l*j2_l)/(2*j1_l*j2_l))*180/3.1456);


j1angle=(atan2(ypos,xpos)-atan2((j2_l*sin(j2angle*radfactor)),(j1_l+j2_l*cos(j2angle*radfactor))))*180/3.1456;


if (j1angle<-180){

    j1angle=180-abs(j1angle+180);

}

if (j1angle<0 && j1angle>=-180){

    j1angle=180+(abs(j1angle+180));

}


if(j2angle>=b_angle_min && j2angle<=b_angle_max && j1angle<=a_angle_max && j1angle>=a_angle_min){

x_pos=xpos;
y_pos=ypos;
a_angle=j1angle;
b_angle=j2angle;

}


}

if(zpos <= 0 && zpos>=-j3_l){

z_pos=zpos;

}

Serial.print(xpos);
Serial.print("   ");
Serial.print(ypos);
Serial.print("   ");
Serial.println(zpos);

goToAngle(a_angle,b_angle,z_pos);

}

void scaraArm::goToAngle(double angleA, double angleB, double zdis){


    a_joint->add_angle(angleA);
    b_joint->add_angle(angleB);
    z_joint->add_angle(zdis);
    a_angle=angleA;
    b_angle=angleB;
    z_pos=zdis;
    matchSpeeds();

}


void scaraArm::goToAngleRel(double angleA, double angleB, double zdis){

    a_angle=a_angle+angleA;
    b_angle=b_angle+angleB;
    z_pos=z_pos+zdis;
    a_joint->add_angle(a_angle);
    b_joint->add_angle(b_angle);
    z_joint->add_angle(z_pos);

    matchSpeeds();

}

void scaraArm::setminmaxangles(double amin, double amax, double bmin, double bmax){

    a_angle_min=amin;
    a_angle_max=amax;
    b_angle_min=bmin;
    b_angle_max=bmax;


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
    y_pos=j1_l+j2_l;
    z_pos=0;

}

void scaraArm::registerJoint(scaraJoint * ajoint, scaraJoint * bjoint, scaraJoint * zjoint){

    a_joint=ajoint;
    b_joint=bjoint;
    z_joint=zjoint;   

}

void scaraArm::createGripper(int epin, double of_x, double of_y, double of_z){

    e_pin=epin;
    e_state=1;
    pinMode(e_pin,OUTPUT);
    j3_l=j3_l-of_z;
    digitalWrite(e_pin,e_state);
    offsetx=of_x;
    offsety=of_y;

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

if(gtype=="M07"){
    e_state=0;
    digitalWrite(e_pin,e_state);

}
if(gtype=="M09"){
    e_state=1;
    digitalWrite(e_pin,e_state);

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
else{
    xpos=x_pos;
}
if(yindex!=-1){
    ypostring=coord.substring(yindex+1,coord.indexOf(" ",yindex));
    ypos=ypostring.toDouble();
}
else{
    ypos=y_pos;
}
if(zindex!=-1){
    zpostring=coord.substring(zindex+1,coord.indexOf(" ",zindex));
    zpos=zpostring.toDouble();
}
else{
    zpos=z_pos;
}
goTo(xpos,ypos,zpos); 


}


