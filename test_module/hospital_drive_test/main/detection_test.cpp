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
// #include "planning.hpp"
// #include "stateMachine.hpp"
// #include "monitoring.hpp"
#include "config.hpp"
// #include "mapping.hpp"
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
    // Planning planning(&worldModel);
    // Mapping mapping(&io, &worldModel);
    Visualizer vis;
    Point p1;
    p1.dist(DIST_SETPOINT);

    // p1.x = 0;
    // p1.y = 0.5;

    // Room corridor;
    Detection_data localDetection;
    Point lexit, rexit;
    // corridor.previousRoom = -1;
    // corridor.roomID = 0;
    // worldModel.set_currentRoom(corridor);

    //    Visualizer vis; //
    //    vis.init_visualize(); //
    bool wall_detected = false;
    bool end_of_program = false;

    int counter = 0;

    while(io.ok()) {

        if (counter == 38) {
            break;
        }


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
            //    if ((localDetection.Exits_total[l].exitPoint1.x == 0 && localDetection.Exits_total[l].exitPoint1.y == 0) || (localDetection.Exits_total[l].exitPoint2.x == 0 && localDetection.Exits_total[l].exitPoint2.y == 0)) {
            //        std::cout << "Problem found. Exit point: " << l << '\n';
            //    }
//                   std::cout << l;
               lexit = localDetection.Exits_total[l].exitPoint1;
               rexit = localDetection.Exits_total[l].exitPoint2;

               if (sqrt(pow(lexit.x - rexit.x, 2) + pow(lexit.y - rexit.y, 2)) < CORRIDOR_EXIT_WIDTH) {
                std::cout << "Exit Distance: " << sqrt(pow(lexit.x - rexit.x, 2) + pow(lexit.y - rexit.y, 2)) << '\n';
                   vis.plotExit(localDetection.Exits_total[l]);
               }


           }

           if(localDetection.Corners_total[l].detected()){
//                   std::cout << l;
                // if (localDetection.Corners_total[l].cornerPoint.x == 0 && localDetection.Corners_total[l].cornerPoint.y == 0) {
                //     std::cout << "Problem found. Corner point: " << l << '\n';
                // }

               vis.plotCorner(localDetection.Corners_total[l]);

           }
        }


        // mapping.execute_mapping();


        if (worldModel.get_closestPointWall().dist() < MIN_DIST_TO_WALL) {
            wall_detected = true;
        }
        else {
            wall_detected = false;
        }

        // std::cout << "Closest Point dist = " << worldModel.get_closestPointWall().dist() << std::endl;
        // std::cout << "Closest Point angle = " << worldModel.get_closestPointWall().angle() << std::endl;

        // low level control
        // end_of_program = state_machine(&worldModel);
        if (wall_detected) {
            p1.x = -0.5;
            p1.y = 0;
            p1.angle(M_PI/2);
            p1.dist(0);

            // worldModel.set_destination(planning.getAwayFromWall());
            std::cout << "WALL DETECTED" << std::endl;
        }
        // else {
        //     p1.x = 0;
        //     p1.y = 0.5;
        //     p1.angle(0);
        //     p1.dist(DIST_SETPOINT);
        //     // monitoring(&worldModel);
        // }
        //
        // else if (counter == 5) {
        //     p1.x = 0.5;
        //     p1.y = 0;
        //     p1.angle(-1*(M_PI/2));
        //     p1.dist(0);
        //
        //     // worldModel.set_destination(planning.getAwayFromWall());
        //     std::cout << "Room 1" << std::endl;
        // }
        else if (counter == 34) {
            p1.x = 0.5;
            p1.y = 0;
            p1.angle(-1*(M_PI/2));
            p1.dist(0);

            // worldModel.set_destination(planning.getAwayFromWall());
            std::cout << "Room 1" << std::endl;
        }
        else if (counter == 35) {
            std::cout << "Paused!" << '\n';
            std::cin.get();

        }
        else {
            p1.x = 0;
            p1.y = 0.5;
            p1.angle(0);
            p1.dist(DIST_SETPOINT);
            // monitoring(&worldModel);
        }

        vis.plotPoint(p1);

        vis.publish();

        worldModel.set_destination(p1);
        worldModel.set_currentLowState(GO_TO_START);

        // std::cout << "Before State machine"<< std::endl;


//        mapping.execute_mapping(&worldModel);

//        switch(worldModel.get_currentHighState()){
//            case: EXPLORE_HOSPITAL

//        }


        pico_drive.driveDecision();

        // if (end_of_program) {
        //     io.speak(talking);
        //     break;
        // }
        counter++;
        std::cout << "Counter Value: " << counter << '\n';

        r.sleep();
        std::cout <<"----------------------------" << std::endl << std::endl;

        // std::cout <<"Enter to continue" << std::endl << std::endl;
        // std::cin.get();
    }


    return 0;
}
