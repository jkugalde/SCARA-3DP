#ifndef scara_arm_h
#define scara_arm_h

#include "Arduino.h"
#include "AccelStepper.h"
#include "Sensor.h"

#define motorInterfaceType 1 // drv8825 or a4988 pololu like

class scaraJoint{

  public:

    double ratio;
    bool inverted;
    int homing_dir;
    double aoffset;
    int offset;
    double angle;
    long position;
    Sensor * sensor;
    long motor_speed;
    AccelStepper * motor;
    double speed = 1.0;
    double accel = 1.0;
    int stepping = 1;

    /**
     * @brief creates joint 
     * 
     * @param a_ratio transmission ratio
     * @param a_inverted inverts movement direction
     * @param a_homing_dir direction for homing
     * @param a_offset offset from home position to initial position
     * @param a_speed max speed
     * @param a_accel acceleration
     * @param a_stepping set microstepping^{-1}
     */

    scaraJoint(double a_ratio,bool a_inverted,int a_homing_dir,double a_offset,double a_speed, double a_accel, int a_stepping);

    /**
     * @brief assigns a value to the current position
     * 
     * @param angle_i in degrees
     */

    void setposangle(double angle_i);

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

    void motor_setup();
    /**
     * @brief Adds an angle to the angle variable, and updates it's position in steps.
     * 
     * @param val 
     */
    void add_angle(double val);

    void setMaxSpeed(double b_speed);

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

    void run();

    long distanceToGo();

    bool isReady();

};


#endif