#include <emc/io.h>
#include <emc/rate.h>
#include <cmath>
#include <iostream>

#include "config.h"
#include "helper.h"
#include "worldModel.h"

#ifndef detection_H
#define detection_H


class Detection{
private:
    emc::IO *inOut;

public:
 Detection(emc::IO *io){
        inOut = io;
        laser = emc::LaserData();

        return;
    }

public:
    Point LatestLaserScan[970]; //Deleted first and last 15 points
    Exit Exits_RL[20];
    Corner Corners_RL[20];
    Exit Exits_LR[20];
    Corner Corners_LR[20];
    Exit Exits_Total[40];
    Corner Corners_Total[40];

    Detection_data detection_data;
    void average_CornersAndExits();
    double aFitPlot;
    double bFitPlot;
    emc::LaserData laser;
    CorridorWalls findCorridorWalls();
    bool getSensorData(); // Method to obtain the sensordata
    void filterLRFData(int nFilterPoints); // Filter data by sensor measurement
    void saveLRFScan();
    bool lineFit(double&, double&, int, int);
    Point findFurthestPoint();
    void findExitsAndCorners_RL();
    void findExitsAndCorners_LR();
    double distance_to_front();
    std::vector<Exit_map> local_Exits();
    Point closest_point();
    Detection_data detection_execution(WorldModel * worldModel);
    void findExitsAndCorners_Final();




   // bool wallDetected(double minDistance);// Method to check if any wall is in the neighbourhood of the robot

};

#endif //detection_H
