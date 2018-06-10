#ifndef driveControl_H
#define driveControl_H

#include <emc/io.h>
#include <emc/rate.h>
#include <iostream>
#include <emc/odom.h>
#include "config.h"

// Destination that is passed to the Control block
typedef struct {
    double x;
    double y;
    double dist;
    double angle;
} Destination;

class DriveControl
        {
private:
    double ref_angle;

    emc::IO *inOut;
    emc::OdometryData odom; // [x,y,a]
    void picoTurnRight();               // Method to let pico turn right
    void picoTurnLeft();                // Method to let pico turn left
    void picoForward();                 // Method to let pico drive forward
    void picoBackwards();               // Method to let pico drive backwards
    void picoSideDrive(double ref_dist, emc::OdometryData odomCur, double angle, emc::Rate *r);   // Method to let pico drive side ways
    void stop();                        // Method to stop moving with the robot

    // complete a turn
    void turn_ref(double ref_angle, emc::OdometryData &odomCur, double angle, emc::Rate *r);
    // drive straight forward/backwards (dependent on bool back)
    void travel_dist(double ref_dist, emc::OdometryData odomCur, emc::Rate *r, bool back);

public:
    DriveControl(emc::IO *io){
        inOut = io;
        odom = emc::OdometryData();

        return;
    }


//    void drive(Low_State low_st, WorldModel* worldModel);  // Method to decide what orientation to drive with
    void picoDrive(Destination *dest);       // Method to let pico turn for an angle and then drive
    void picoDriveBackwards(Destination * dest); // Method to let pico turn for an angle and then drive backwards
};

#endif //driveControl_H
