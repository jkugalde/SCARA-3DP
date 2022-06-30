#ifndef scaraArm_h
#define scaraArm_h

#include "Arduino.h"
#include "scaraJoint.h"


class scaraArm{
  public:
  
    int num_joints;
    int idx;
    int  m_idx;
    long * l_positions;
    scaraJoint * joints;
    AccelStepper * motors;
    Sensor * sensors;
    int num_motors;

    scaraArm(int n_joints);

};


#endif