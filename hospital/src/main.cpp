#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>
#include <cstdlib>


#include "driveControl.h"
#include "detection.h"
#include "worldModel.h"
#include "planning.h"
#include "stateMachine.h"
#include "monitoring.h"
#include "config.h"
using namespace std;


void initialize(Detection_data *data, Destination *dest, Flags *flags){

    // Flags

    flags->in_corridor = false;
    flags->turn = false;
    flags->turned_once = false;
    flags->escaped = false;
    flags->drive_frw = false;



    // Destination

    dest->angle = 0;
    dest->dist = 0;



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
    emc::Rate r(2); //EXECUTION_RATE
    emc::IO io;
    //    emc::OdometryData odom;

    // Transition values
    Detection_data data;
    Destination dest;
    Destination far_point;
    Flags flags;
    initialize(&data, &dest, &flags);



    // Initialize the Classes
    DriveControl pico_drive(&io);
    Detection detection(&r, &io, &flags);



    while(io.ok()) {

        cout <<"----------------------------" << endl << endl;

        detection();
        monitoring();
        state_machine(high_st, low_st);
        drive();

        r.sleep();
        /*std::cout << "Press Enter to continue ..." << '\n';
        cin.get();*/
    }


    return 0;
}
