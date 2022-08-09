#ifndef scaraArm_h
#define scaraArm_h

#include "Arduino.h"
#include "scaraJoint.h"
#include "Math.h"


class scaraArm{
  public:
  
    AccelStepper * motors;
    Sensor * sensors;
    scaraJoint * a_joint;
    scaraJoint * b_joint;
    scaraJoint * z_joint;
    double x_pos;
    double y_pos;
    double z_pos;
    double a_angle;
    double b_angle;
    int e_pin;
    bool e_state;
    double j1_l;
    double j2_l;
    double j1pos[2]={0,0};
    double j2pos[2]={0,0};
    double radfactor = 3.1459/180;

    scaraArm(double j1l, double j2l);

    String where();

    void setposangle(double angle_i);

    void goTo(double xpos, double ypos, double zpos);

    void homeAxes();

    void registerJoint(scaraJoint * ajoint, scaraJoint * bjoint, scaraJoint * zjoint);

    void createGripper(int epin);

    void run();

    void goToAngle(double angleA=0, double angleB=0, double zdis=0);

    void goToAngleRel(double angleA=0, double angleB=0, double zdis=0);

    void setSpeeds(double speeda, double speedb, double speedz);

    void matchSpeeds();

    bool isReady();

    void gcode(String msg);

    void g1(String coord);


};



#endif