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
#include "mapping.h"
#include "helper.h"


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

    Room corridor;

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
        mapping.execute_mapping();


        //        if(detection.getSensorData()) {

        //            detection.saveLRFScan();
        //            int index = 0;


        //            detection.average_CornersAndExits();
        //            //detection.findExitsAndCorners_LR();


        //            // UPDATE VISUALIZER

        //            vis.init_visualize();
        //            for(unsigned int i = 0; i < 970 ; ++i)
        //            {
        //                double x = detection.LatestLaserScan[i].x;
        //                double y = detection.LatestLaserScan[i].y;
        //                vis.plot_xy_color(x, y,0, 0, 255);
        //            }
        //            for(unsigned int l = 0; l < 20; l=l+1){
        //               if(detection.Exits_RL[l].detected){
        //                   std::cout << l;
        //                   vis.plotExit(detection.Exits_RL[l]);

        //               }
        //            }
        //            for(unsigned int l = 0; l < 20; l=l+1){
        //               if(detection.Corners_RL[l].detected){
        //                   vis.plotCorner(detection.Corners_RL[l]);

        //               }
        //            }

        //            for(unsigned int l = 0; l < 20; l=l+1){
        //               if(detection.Exits_LR[l].detected){
        //                   vis.plotExit_LR(detection.Exits_LR[l]);

        //               }
        //            }
        //            for(unsigned int l = 0; l < 20; l=l+1){
        //               if(detection.Corners_LR[l].detected){
        //                   vis.plotCorner_LR(detection.Corners_LR[l]);

        //               }
        //            }

        //            //vis.plotExit(exit);
        //            vis.plotLine(detection.aFitPlot,detection.bFitPlot,exit.exitPoint1,exit.exitPoint2);
        //            vis.publish();


        //            //END UPDATE VISUALIZER

        //        }

        if (worldModel.get_closestPointWall().dist() < MIN_DIST_TO_WALL) {
            wall_detected = true;
        }
        else {
            wall_detected = false;
        }

        std::cout << "Closest Point dist = " << worldModel.get_closestPointWall().dist() << std::endl;
        std::cout << "Closest Point angle = " << worldModel.get_closestPointWall().angle() << std::endl;

        // low level control
        if (wall_detected) {
            worldModel.set_destination(planning.getAwayFromWall());
            std::cout << "WALL DETECTED" << std::endl;
        }
        else {
            end_of_program = state_machine(&worldModel);
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

        /*std::cout << "Press Enter to continue ..." << '\n';
        cin.get();*/
    }


    return 0;
}
