#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <string>


#include "driveControl.hpp"
#include "detection.hpp"
#include "worldModel.hpp"
#include "planning.hpp"
#include "stateMachine.hpp"
#include "monitoring.hpp"
#include "config.hpp"
#include "mapping.hpp"
#include "helper.hpp"
#include "visualize.hpp"


int main ()
{
    // Initialization of Robot
    emc::Rate r(EXECUTION_RATE); //EXECUTION_RATE
    emc::IO io;
    //    emc::OdometryData odom;

    // Initialize the Classes
    WorldModel worldModel;
    DriveControl pico_drive(&io, &worldModel);
    Detection detection(&io, &worldModel); //
    Planning planning(&worldModel);
    Mapping mapping(&io, &worldModel);
    Visualizer vis;

    Room corridor;
    Detection_data localDetection;

    corridor.previousRoom = -1;
    corridor.roomID = 0;
    worldModel.set_currentRoom(corridor);

    //    Visualizer vis; //
    //    vis.init_visualize(); //
    bool wall_detected = false;
    bool end_of_program = false;

    std::string talking = "I am parked";

    while(io.ok()) {


        detection.detection_execution(); //
        localDetection = worldModel.get_localDetection();

        vis.init_visualize();
        for(unsigned int i = 0; i < 970 ; ++i)
        {
            double x = detection.LatestLaserScan[i].x;
            double y = detection.LatestLaserScan[i].y;
            vis.plot_xy_color(x, y,0, 0, 255);
        }

        for(unsigned int l = 0; l < 40; l=l+1){
           if(localDetection.Exits_total[l].detected()){
//                   std::cout << l;
               vis.plotExit(localDetection.Exits_total[l]);

           }

           if(localDetection.Corners_total[l].detected()){
//                   std::cout << l;
               vis.plotCorner(localDetection.Corners_total[l]);

           }
        }

        vis.publish();


        mapping.execute_mapping();


        if (worldModel.get_closestPointWall().dist() < MIN_DIST_TO_WALL) {
            wall_detected = true;
        }
        else {
            wall_detected = false;
        }

        std::cout << "Closest Point dist = " << worldModel.get_closestPointWall().dist() << std::endl;
        std::cout << "Closest Point angle = " << worldModel.get_closestPointWall().angle() << std::endl;

        // low level control
        end_of_program = state_machine(&worldModel);
        if (wall_detected) {
            worldModel.set_destination(planning.getAwayFromWall());
            std::cout << "WALL DETECTED" << std::endl;
        }
        else {
            monitoring(&worldModel);
        }

        std::cout << "Before State machine"<< std::endl;


//        mapping.execute_mapping(&worldModel);

//        switch(worldModel.get_currentHighState()){
//            case: EXPLORE_HOSPITAL

//        }


        pico_drive.driveDecision();

        if (end_of_program) {
            io.speak(talking);
            break;
        }

        r.sleep();
        std::cout <<"----------------------------" << std::endl << std::endl;

    }


    return 0;
}
