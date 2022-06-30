#include "Arduino.h"
#include "scaraArm.h"


scaraArm::scaraArm(int n_joints){

    num_joints = n_joints;
    joints = (scaraJoint *)malloc(sizeof(scaraJoint *)*n_joints);
    sensors =(Sensor *) malloc(sizeof(Sensor *)*n_joints);
    l_positions = (long *) malloc(sizeof(long)*n_joints);
    for(int i =0;i<num_joints;i++){
        l_positions[i]=0;
    }

    idx=0;
    m_idx =0;
    num_motors =0;

}
