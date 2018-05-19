#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>


#include "driveControl.cpp"
#include "detection.cpp"
#include "worldModel.cpp"

#include "config.h"

/*typedef enum {
    drive_forward = 1,
    drive_backward,
    rotate,
} state_t;*/

int main(int argc, char *argv[])
{
    // Initialization of Robot
    emc::Rate r(EXECUTION_RATE);
    emc::IO io;
    emc::OdometryData odom;

    // Initialize the Classes
    DriveControl pico_drive(&io);
    Detection detection(&io);
    WorldModel worldModel;


    int nFilterPoints = 15;

    while(io.ok()) {
        //pico_drive.driveForward(1.0);
        if(detection.getSensorData()) {
            std::cout << detection.laser.ranges.size() << std::endl;
            std::cout << detection.laser.ranges[999] << "  ";
            std::cout << detection.laser.ranges[984] << std::endl;

            detection.filterLRFData(&detection.laser, nFilterPoints);

            std::cout << detection.laser.ranges[999] << "  ";
            std::cout << detection.laser.ranges[984] << std::endl;

        }


        r.sleep();
    }


    return 0;
}
