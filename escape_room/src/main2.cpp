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
        if(detection.get_data(&r)) {

//            if (data.exit.detected){
//                flags.in_corridor = true;
//            }

            Detection detection(&r, &io, &flags);
            //should be function to update flags instead
            data = detection.get_Detection_data();

            if (data.exit.detected){
                cout << "exit detected: true" << endl;
            } else{
                cout << "exit detected: false " << endl;
            }

//            if (flags.in_corridor){
//                data.exit.detected = false;
//            }

            cout << "exit Point 1 (x,y) " << data.exit.exitPoint_det1.x <<" "<< data.exit.exitPoint_det1.y <<endl;
            cout << "exit Point 2 (x,y) " << data.exit.exitPoint_det2.x <<" "<< data.exit.exitPoint_det2.y <<endl;

            cout << "Furthest point (" << data.furthest_point.x << " " << data.furthest_point.y << ")" <<"\n" << "\n";

            cout << "flag in corridor" << flags.in_corridor << "\n";

            Planning plan(&data, &flags, &far_point);
            dest = plan.get_Destination();


            cout << "Relative angle " << dest.angle << endl << endl;

            pico_drive.picoDrive(&dest, &flags);

            cout <<"----------------------------" << endl << endl;
        }


        r.sleep();
    }


    return 0;
}
