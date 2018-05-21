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
#include "initialization.cpp"



int main(int argc, char *argv[])
{
    // Initialization of Robot
    emc::Rate r(EXECUTION_RATE);
    emc::IO io;
    emc::OdometryData odom;

    // Initialize the Classes
    DriveControl pico_drive(&io);
    Detection detection(&io);

    Detection_data data;

    Destination dest;
    Flags flags;



    //-----------------------------------------------
    //------add initialization of parameters---------
    //-----------------------------------------------
    initialize(&data, &dest, &flags);


    while(io.ok()) {
        if(detection.getSensorData()) {

            detection.saveLRFScan(&detection.laser);

            // check if escaped the room
            if (corridor.escaped){
                break;
            }
            detection_general(&data,&flags);
            dest = planning(&data, &dest, &flags);
            picoDrive(dest);

        }


        r.sleep();
    }


    return 0;
}
