#ifndef scara_arm_h
#define scara_arm_h

#include "Arduino.h"
#include "AccelStepper.h"
#include "Sensor.h"


#define MICRO_STEPPING 4
#define ACC 10000
#define SPEED_CONSTANT 800// Speed base mutliplier for all joints.
#define motorInterfaceType 1

class scaraJoint{
  public:
    int index;
    double ratio;
    bool inverted;
    int homing_dir;
    int offset;
    long angle;
    long position;
    int jn_steppers;
    Sensor * sensor;
    long motor_speed;
    AccelStepper * motor;
    double speed_multiplier =1.0;

    scaraJoint(double a_ratio,bool a_inverted,int a_homing_dir,int a_offset,double a_speed_multiplier);

    /**
     * @brief Creates a new accelstepper motor for the joint.
     * 
     * @param step_pin step pin of the new motor
     * @param dir_pin direction pin of the new motor
     */
    void create_motor(int step_pin,int dir_pin);
    /**
     * @brief Creates a new sensor for the joint
     * 
     * @param pin 
     */
    void create_sensor(int pin);
    /**
     * @brief Prints to serial a string representation of the joint
     * 
     */
    void show();
    /**
     * @brief Runs the comands to setup the motors of the joint.
     * 
     */
    void motor_setup();
    /**
     * @brief Adds an angle to the angle variable, and updates it's position in steps.
     * 
     * @param val 
     */
    void add_angle(long val);
    /**
     * @brief Statrs the homing proces for the joint.
     * 
     */
    void launch_home();
    /**
     * @brief Runs the homing procedure of the joint.
     * 
     * @param m Multistepper used to run the other steppers.
     */
    void home();
    /**
     * @brief Sets a new offset for the joint
     * 
     * @param new_offset new value for offset
     */
    void set_offset(long new_offset);
    /**
     * @brief prints the offset of the joint.
     * 
     */
    void show_offset();

    void run();

    long distanceToGo();

    bool isReady();

};


#endif