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
    Corner corner;
    Point p1, p2;

    p1.dist(0.5);
    // Room corridor;
    // Detection_data localDetection;
    //
    // corridor.previousRoom = -1;
    // corridor.roomID = 0;
    // worldModel.set_currentRoom(corridor);

    //    Visualizer vis; //
    //    vis.init_visualize(); //
    // bool wall_detected = false;
    // bool end_of_program = false;

    // std::string talking = "I am parked";

    int counter = 0;
    corner.cornerPoint.x = 0;
    corner.cornerPoint.y = 0.5;
    corner.detected(true);

    while(io.ok()) {


        detection.detection_execution(); //
        // localDetection = worldModel.get_localDetection();


        worldModel.set_destination(p1);
        worldModel.set_currentLowState(GO_TO_START);

        vis.init_visualize();
        for(unsigned int i = 0; i < 970 ; ++i)
        {
            double x = detection.LatestLaserScan[i].x;
            double y = detection.LatestLaserScan[i].y;
            vis.plot_xy_color(x, y,0, 0, 255);
        }

        vis.plotCorner(corner);


        vis.publish();

        p2 = worldModel.get_destination();

        std::cout << "x: " << p1.x << " y: " << p1.y << " a: " << p1.angle() << " d: " << p1.dist() << '\n';
        std::cout << "x: " << p2.x << " y: " << p2.y << " a: " << p2.angle() << " d: " << p2.dist() << '\n';

        // std::cout << "x: " << pico_drive.dest.x << " y: " << pico_drive.dest.y << " a: " << pico_drive.dest.angle() << " d: " << pico_drive.dest.dist() << '\n';

        pico_drive.driveDecision();

        // low level control


//        mapping.execute_mapping(&worldModel);

//        switch(worldModel.get_currentHighState()){
//            case: EXPLORE_HOSPITAL

//        }
        if (counter == 4) {
            corner.cornerPoint.x = -0.5;
            corner.cornerPoint.y = 0;
            corner.detected(true);
            p1.angle(M_PI/2);
        }
        else if (counter == 7) {
            break;
        }
        else {
            corner.cornerPoint.x = 0;
            corner.cornerPoint.y = 0.5;
            corner.detected(true);
            p1.angle(0);
        }


        // pico_drive.driveDecision();


        r.sleep();
        std::cout <<"----------------------------" << std::endl << std::endl;
        // std::cout <<"Press Enter to continue" << std::endl << std::endl;
        //
        // std::cin.get();

        counter++;

    }


    return 0;
}
