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

    // Transition values
    Detection_data data;
    Destination dest;
    Flags flags;
    initialize(&data, &dest, &flags);

    // Initialize the Classes
    DriveControl pico_drive(&io);
    Detection detection(&io, &flags);



    while(io.ok()) {
        if(detection.getSensorData()) {

    /*        // check if escaped the room
            if (flags.escaped){
                break;
            }*/
            Detection detection(&io, &flags);
            data = detection.get_Detection_data();

            Planning plan(&data, &flags);
            dest = plan.get_Destination();


            pico_drive.picoDrive(dest.angle);

        }


        r.sleep();
    }


    return 0;
}
