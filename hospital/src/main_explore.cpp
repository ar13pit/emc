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


int main(int argc, char *argv[])
{
    // Initialization of Robot
    emc::Rate r(EXECUTION_RATE); //EXECUTION_RATE
    emc::IO io;
    //    emc::OdometryData odom;
    // Initialize the Classes

    WorldModel worldModel;
    DriveControl pico_drive(&io);
    Detection detection(&io); //
    Planning planning;
    Mapping mapping(&io);


    Point initial_point;
    initial_point.angle = 0; initial_point.dist = 0; initial_point.x = 0; initial_point.y = 0;

    Point_map initial_point_map;
    initial_point_map.x = 0; initial_point_map.y = 0;
    std::vector<Point_map> initial_point_map_vec;

    Exit_map initial_exit_map;
    initial_exit_map.point1 = initial_point_map;
    initial_exit_map.point2 = initial_point_map;
    std::vector<Exit_map> initial_exit_map_vec;

    Room corridor;
    //    corridor.corners = initial_point_map_vec;
    corridor.exit = initial_exit_map;
    corridor.previousRoom = -1;
    corridor.roomID = 0;


    Detection_data initial_detection_data;
    initial_detection_data.closest_Point = initial_point;
    initial_detection_data.Corners_total[40] = {};
    initial_detection_data.Exits_total[40] = {};
    initial_detection_data.local_exits = initial_exit_map_vec;
    worldModel.set_localDetection(initial_detection_data);


    worldModel.set_currentHighState(EXPLORE_HOSPITAL);
    worldModel.set_currentLowState(EXPLORE_CORRIDOR);
    // worldModel.setAllDetectedExits(initial_exit_map_vec);
    //    worldModel.set_closestPointWall(initial_point);
    worldModel.set_currentLocation(IN_CORRIDOR);
    worldModel.set_currentRoom(corridor.roomID);
    worldModel.set_curRoom(corridor);
    worldModel.set_globalPosition(initial_point_map);




    //    Visualizer vis; //
    //    vis.init_visualize(); //
    bool wall_detected = false;
    bool end_of_program = false;
    std::string talking = "I am parked";

    while(io.ok()) {


        detection.detection_execution(&worldModel); //

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

        if (worldModel.get_closestPointWall().dist < MIN_DIST_TO_WALL) {
            wall_detected = true;
        }
        else
            wall_detected = false;

        // low level control
        if (wall_detected){
            worldModel.set_destination(planning.getAwayFromWall(&worldModel));
            std::cout << "WALL DETECTED" << std::endl;
        } else {
            monitoring(&worldModel);
        }
        std::cout << "Before State machine"<< std::endl;


//        mapping.execute_mapping(&worldModel);
        end_of_program = state_machine(&worldModel);
//        switch(worldModel.get_currentHighState()){
//            case: EXPLORE_HOSPITAL

//        }


        pico_drive.driveDecision(&worldModel);

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
