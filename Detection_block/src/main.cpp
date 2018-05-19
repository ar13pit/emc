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



    while(io.ok()) {
        //pico_drive.driveForward(1.0);
        if(detection.getSensorData()) {

            detection.saveLRFScan(&detection.laser);

            std::cout << detection.LatestLaserScan[969].x << "  ";
            std::cout << detection.LatestLaserScan[969].y << std::endl;

            std::cout << detection.LatestLaserScan[969].angle << "  ";
            std::cout << detection.LatestLaserScan[969].dist << std::endl << std::endl;

        }


        r.sleep();
    }


    return 0;
}
