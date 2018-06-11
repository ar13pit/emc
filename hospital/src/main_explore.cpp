#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <string>


#include "driveControl.h"
#include "detection.h"
#include "worldModel.h"
#include "planning.h"
#include "stateMachine.h"
#include "monitoring.h"
#include "config.h"
using namespace std;


int main(int argc, char *argv[])
{
    // Initialization of Robot
    emc::Rate r(EXECUTION_RATE); //EXECUTION_RATE
    emc::IO io;
    //    emc::OdometryData odom;

    // Transition values
    WorldModel worldModel;
    High_State high_st = EXPLORE_HOSPITAL;
    Low_State low_st = EXPLORE_CORRIDOR;


    // Initialize the Classes
    DriveControl pico_drive(&io);
    Detection detection(&r, &io, &flags);

    bool end_of_program = false;
    string talking = "I am parked";

    while(io.ok()) {



        detection();

        // low level control
        if (wall_detected){
            worldModel.set_destination(Planning::getAwayFromWall(low_st));
            std::cout << "WALL DETECT"<<  dest.angle << std::endl;
        } else {
            monitoring(&worldModel);
        }

        end_of_program = state_machine(high_st, low_st, &worldModel);

        pico_drive.driveDecision(low_st,&worldModel);

        if (end_of_program) {
            io.speak(&talking);
        }

        r.sleep();
        cout <<"----------------------------" << endl << endl;

        /*std::cout << "Press Enter to continue ..." << '\n';
        cin.get();*/
    }


    return 0;
}
