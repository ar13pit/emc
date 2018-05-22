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

using namespace std;

void initialize(Detection_data *data, Destination *dest, Flags *flags){

    // Flags

    flags->in_corridor = false;
    flags->in_process = false;
    flags->turned_once = false;
    flags->escaped = false;



    // Destination

    dest->angle = 0;
    dest->x = 0;
    dest->y = 0;



    // Detection data

    //corridor initialization
    data->corridor.escaped = false;

    data->corridor.leftWall1.angle = 0;
    data->corridor.leftWall1.dist = 0;
    data->corridor.leftWall1.x = 0;
    data->corridor.leftWall1.y = 0;

    data->corridor.leftWall2.angle = 0;
    data->corridor.leftWall2.dist = 0;
    data->corridor.leftWall2.x = 0;
    data->corridor.leftWall2.y = 0;

    data->corridor.rightWall1.angle = 0;
    data->corridor.rightWall1.dist = 0;
    data->corridor.rightWall1.x = 0;
    data->corridor.rightWall1.y = 0;

    data->corridor.rightWall2.angle = 0;
    data->corridor.rightWall2.dist = 0;
    data->corridor.rightWall2.x = 0;
    data->corridor.rightWall2.y = 0;


    // exit initialization
    data->exit.detected = false;

    data->exit.exitPoint_det1.angle = 0;
    data->exit.exitPoint_det1.dist = 0;
    data->exit.exitPoint_det1.x = 0;
    data->exit.exitPoint_det1.y = 0;

    data->exit.exitPoint_det2.angle = 0;
    data->exit.exitPoint_det2.dist = 0;
    data->exit.exitPoint_det2.x = 0;
    data->exit.exitPoint_det2.y = 0;


    // furthest point initialization
    data->furthest_point.angle = 0;
    data->furthest_point.dist = 0;
    data->furthest_point.x = 0;
    data->furthest_point.y = 0;


}


int main(int argc, char *argv[])
{
    // Initialization of Robot
    emc::Rate r(1); //EXECUTION_RATE
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

            cout << "exit (x,y)" << data.exit.exitPoint_det1.x <<" "<< data.exit.exitPoint_det1.y <<endl;

            Planning plan(&data, &flags);
            dest = plan.get_Destination();


            cout << "Relative angle " << dest.angle << endl << endl;
          //  pico_drive.picoDrive(dest.angle, &flags);
            //usleep(1000000);
        }


        r.sleep();
    }


    return 0;
}


