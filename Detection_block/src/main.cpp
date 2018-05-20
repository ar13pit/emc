#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>


#include "driveControl.cpp"
#include "detection.cpp"
#include "worldModel.cpp"

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

    // Initialize the Classes
    DriveControl pico_drive(&io);
    Detection detection(&io);
    WorldModel worldModel;



    while(io.ok()) {
        //pico_drive.driveForward(1.0);
        if(detection.getSensorData()) {

            detection.saveLRFScan(&detection.laser);
            int index = 0;
            /*std::cout << detection.LatestLaserScan[index].x << "  ";
            std::cout << detection.LatestLaserScan[index].y << std::endl;

            std::cout << detection.LatestLaserScan[index].angle << "  ";
            std::cout << detection.LatestLaserScan[index].dist << std::endl << std::endl;

            double aFit = 0;
            double bFit = 0;
            int firstPoint = 10;
            int lastPoint = 80;

            if (detection.lineFit(aFit, bFit, firstPoint, lastPoint) ){
               std::cout << "Line was found, formula: y = " << aFit << "x + "<< bFit << std::endl;
            }else{
               std::cout << "Line was not found..." << std::endl;
            }*/

            std::cout << "Furthest point: (" << detection.findFurthestPoint().x << ", " << detection.findFurthestPoint().y << ")" << std::endl << std::endl;

            CorridorWalls walls;
            walls = detection.findCorridorWalls();
            if(walls.escaped){
                std::cout << "No corridor found to follow" << std:: endl;
            }else{
                std::cout << "Corridor still spotted!" << std::endl;
            }

            //pico_drive.driveBackward(0.1);
        }


        r.sleep();
    }


    return 0;
}
