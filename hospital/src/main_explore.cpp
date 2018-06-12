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
//#include "visualize.h"
using namespace std;


int main(int argc, char *argv[])
{
    // Initialization of Robot
    emc::Rate r(EXECUTION_RATE); //EXECUTION_RATE
    emc::IO io;
    //    emc::OdometryData odom;

    // Transition values
    WorldModel worldModel;
    worldModel.set_currentHighState(EXPLORE_HOSPITAL);
    worldModel.set_currentLowState(EXPLORE_CORRIDOR);


    // Initialize the Classes
    DriveControl pico_drive(&io);
    Detection detection(&io); //
    Planning planning;


//    Visualizer vis; //
//    vis.init_visualize(); //

    bool end_of_program = false;
    string talking = "I am parked";

    while(io.ok()) {

        detection(); //

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


        // low level control
        if (wall_detected){
            worldModel.set_destination(planning->getAwayFromWall(low_st));
            std::cout << "WALL DETECTED"<<  dest.angle << std::endl;
        } else {
            monitoring(&worldModel);
        }

        end_of_program = state_machine(&worldModel);

        pico_drive.driveDecision(low_st,&worldModel);

        if (end_of_program) {
            io.speak(&talking);
            break;
        }

        r.sleep();
        cout <<"----------------------------" << endl << endl;

        /*std::cout << "Press Enter to continue ..." << '\n';
        cin.get();*/
    }


    return 0;
}
