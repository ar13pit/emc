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
    Exit exit;

    while(io.ok()) {
        //pico_drive.driveForward(1.0);
        if(detection.getSensorData()) {

            detection.saveLRFScan();
            int index = 0;

            exit = detection.findExit();

            if (exit.detected){
                std::cout << "Exit detected at: (" << exit.exitPoint1.x <<", " << exit.exitPoint1.y << ") and (" << exit.exitPoint2.x <<", " << exit.exitPoint2.y << ")" << std::endl;
             }
            else{
                std::cout << "No exit detected..." << std::endl;
            }


            //pico_drive.driveBackward(0.1);
        }


        r.sleep();
    }


    return 0;
}
