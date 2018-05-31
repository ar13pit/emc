#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>


//#include "driveControl.cpp"
#include "detection.h"
//#include "worldModel.cpp"
#include "visualize.h"

#include "config.h"

/*typedef enum {
    drive_forward = 1,
    drive_backward,
    rotate,
} state_t;*/

int main(int argc, char *argv[])
{
    // Initialization of Robot
    emc::Rate r(EXECUTION_RATE);
    emc::IO io;
    emc::OdometryData odom;

    // Initialize the Classesr
    Detection detection(&io);
    Exit exit;
    Visualizer vis;

    vis.init_visualize();

    while(io.ok()) {
        //pico_drive.driveForward(1.0);
        if(detection.getSensorData()) {

            detection.saveLRFScan();
            int index = 0;

            exit = detection.findExit();

            if (exit.detected){
                std::cout << "Exit detected at: (" << exit.exitPoint1.x <<", " << exit.exitPoint1.y << ") and (" << exit.exitPoint2.x <<", " << exit.exitPoint2.y << ")" << std::endl;
             }
            else{
                std::cout << "No exit detected..." << std::endl;
            }


            // UPDATE VISUALIZER

            vis.init_visualize();
            for(unsigned int i = 0; i < 970 ; ++i)
            {
                double x = detection.LatestLaserScan[i].x;
                double y = detection.LatestLaserScan[i].y;
                vis.plot_xy_color(x, y,0, 0, 255);
            }
            vis.publish();

            //END UPDATE VISUALIZER


        }


        r.sleep();
    }


    return 0;
}
