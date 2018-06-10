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

    data->exit.exitPoint1.angle = 0;
    data->exit.exitPoint1.dist = 0;
    data->exit.exitPoint1.x = 0;
    data->exit.exitPoint1.y = 0;

    data->exit.exitPoint2.angle = 0;
    data->exit.exitPoint2.dist = 0;
    data->exit.exitPoint2.x = 0;
    data->exit.exitPoint2.y = 0;


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
    WorldModel worldModel;
    High_State high_st;
    Low_State low_st;


    // Initialize the Classes
    DriveControl pico_drive(&io);
    Detection detection(&r, &io, &flags);



    while(io.ok()) {



        detection();

        // low level control
        if (wall_detected){
            worldModel.set_destination(Planning::getAwayFromWall(low_st));
            std::cout << "WALL DETECT"<<  dest.angle << std::endl;
        } else {
            monitoring(&worldModel);
        }

        state_machine(high_st, low_st, &worldModel);

        pico_drive.drive(low_st,&worldModel);

        r.sleep();
        cout <<"----------------------------" << endl << endl;

        /*std::cout << "Press Enter to continue ..." << '\n';
        cin.get();*/
    }


    return 0;
}
