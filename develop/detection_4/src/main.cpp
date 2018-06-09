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


            detection.findExitsAndCorners();
            detection.findExitsAndCorners_LR();


            // UPDATE VISUALIZER

            vis.init_visualize();
            for(unsigned int i = 0; i < 970 ; ++i)
            {
                double x = detection.LatestLaserScan[i].x;
                double y = detection.LatestLaserScan[i].y;
                vis.plot_xy_color(x, y,0, 0, 255);
            }
//            for(unsigned int l = 0; l < 20; l=l+1){
//               if(detection.Exits[l].detected){
////                   std::cout << l;
//                   vis.plotExit(detection.Exits[l]);

//               }
//            }
//            for(unsigned int l = 0; l < 20; l=l+1){
//               if(detection.Corners[l].detected){
//                   vis.plotCorner(detection.Corners[l]);

//               }
//            }

            for(unsigned int l = 0; l < 20; l=l+1){
               if(detection.Exits_LR[l].detected){
//                   std::cout << l;
                   vis.plotExit_LR(detection.Exits_LR[l]);

               }
            }
            for(unsigned int l = 0; l < 20; l=l+1){
               if(detection.Corners_LR[l].detected){
                   vis.plotCorner_LR(detection.Corners_LR[l]);

               }
            }

            //vis.plotExit(exit);
            vis.plotLine(detection.aFitPlot,detection.bFitPlot,exit.exitPoint1,exit.exitPoint2);
            vis.publish();


            //END UPDATE VISUALIZER

        }


       r.sleep();
    }


    return 0;
}
