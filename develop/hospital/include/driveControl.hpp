#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <iostream>

#include "config.hpp"
#include "helper.hpp"
#include "worldModel.hpp"

#ifndef driveControl_H
#define driveControl_H


class DriveControl {

    emc::IO *inOut;
    emc::OdometryData odomRef;
    emc::OdometryData odomCur;          // [x,y,a]
    emc::Rate r;

    WorldModel* WM;

    Point dest;

    double ref_angle;

    Low_State low_st;

    void picoTurnRight();               // Method to let pico turn right
    void picoTurnLeft();                // Method to let pico turn left
    void picoForward();                 // Method to let pico drive forward
    void picoBackwards();               // Method to let pico drive backwards
//    void picoSideDrive(double ref_dist, emc::OdometryData odomCur, double angle, emc::Rate *r);   // Method to let pico drive side ways
    void picoStop();                    // Method to stop moving with the robot
    bool calibrate_angle();
    void picoDrive(bool back);       // Method to let pico turn for an angle and then drive
    void picoTurning(); // Method to let pico turn for an angle and then drive backwards
    void picoSideDrive();


public:
    DriveControl (emc::IO* io, WorldModel* worldmodel);

//    void drive(Low_State low_st, WorldModel* worldModel);  // Method to decide what orientation to drive with

    bool driveDecision ();
};

#endif //driveControl_H
