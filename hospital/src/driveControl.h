#ifndef driveControl_H
#define driveControl_H

#include <emc/io.h>
#include <emc/rate.h>
#include "main2.hpp"
#include <iostream>
#include "planning.h"
#include "config.h"

class DriveControl
        {
private:
    double ref_angle;

    emc::IO *inOut;
    emc::OdometryData odom; // [x,y,a]
    void picoTurnRight();               // Method to let pico turn right
    void picoTurnLeft();                // Method to let pico turn left
    void picoForward();                 // Method to let pico drive forward
    void picoSideDrive(double ref_dist, emc::OdometryData odomCur, double angle, emc::Rate *r);   // Method to let pico drive side ways
    void stop();                        // Method to stop moving with the robot
    void turn_ref(double ref_angle, emc::OdometryData odomCur, double angle, emc::Rate *r);
    void travel_dist(double ref_dist, emc::OdometryData odomCur, emc::Rate *r);

public:
    DriveControl(emc::IO *io){
        inOut = io;
        odom = emc::OdometryData();

        return;
    }

    void picoDrive(Destination *dest);       // Main method to let pico turn for an angle and then drive

};

#endif //driveControl_H
