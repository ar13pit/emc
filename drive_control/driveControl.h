#ifndef driveControl_H
#define driveControl_H

#include <emc/io.h>

class DriveControl
        {
private:
    emc::IO *inOut;
    emc::OdometryData odom; // [x,y,a]

public:
    DriveControl(emc::IO *io){
        inOut = io;
        odom = emc::OdometryData();

        return;
    }

    void picoDrive(double angle); // Main method to let pico turn for an angle and then drive
    void picoTurnRight(); // Method to let pico turn right
    void picoTurnLeft(); // Method to let pico turn left
    void picoForward(); // Method to let pico drive forward
    void picoSideDrive(double angle); // Method to let pico drive side ways
    void stop(); // Method to stop moving with the robot
};

#endif //driveControl_H
