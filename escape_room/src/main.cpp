#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>


#include "driveControl.h"
#include "detection.h"
#include "worldModel.h"
#include "planning.h"

#include "config.h"

int main(int argc, char *argv[])
{
    // Initialization of Robot
    emc::Rate r(EXECUTION_RATE);
    emc::IO io;
    emc::OdometryData odom;

    // Initialize the Classes
    DriveControl pico_drive(&io);
    Detection detection(&io);
    Exit exit;


    while(io.ok()) {
        if(detection.getSensorData()) {

            detection.saveLRFScan(&detection.laser);
            int index = 0;

            exit = detection.findExit();

        }


        r.sleep();
    }


    return 0;
}
