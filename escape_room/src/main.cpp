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

    Detection_data data;

    Destination dest;
    bool in_process;
    bool in_corridor = false;

    //-----------------------------------------------
    //------add initialization of parameters---------
    //-----------------------------------------------
    // corridor.escaped = false


    while(io.ok()) {
        if(detection.getSensorData()) {

            detection.saveLRFScan(&detection.laser);

            // check if escaped the room
            if (corridor.escaped){
                break;
            }
//------------------------------------
//------needs to be optimized---------
//------------------------------------
            detection_general(&data,in_corridor);
            dest = planning(&data, &dest, in_corridor);

        }


        r.sleep();
    }


    return 0;
}
