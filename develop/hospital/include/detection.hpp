#include <emc/io.h>
#include <emc/rate.h>
#include <cmath>
#include <iostream>

#include "config.hpp"
#include "helper.hpp"
#include "worldModel.hpp"

#ifndef detection_H
#define detection_H


class Detection {

    emc::IO* inOut;

    WorldModel* WM;

public:
    emc::LaserData laser;

    Point LatestLaserScan[970]; //Deleted first and last 15 points

    Exit Exits_RL[20];
    Exit Exits_LR[20];
    Exit Exits_Total[40];

    Corner Corners_RL[20];
    Corner Corners_LR[20];
    Corner Corners_Total[40];

    Detection_data detection_data;

    double aFitPlot;
    double bFitPlot;

    Detection(emc::IO* io, WorldModel* worldmodel);

    void average_CornersAndExits();
    CorridorWalls findCorridorWalls();
    bool getSensorData(); // Method to obtain the sensordata
    void filterLRFData(int nFilterPoints); // Filter data by sensor measurement
    void saveLRFScan();
    bool lineFit(double&, double&, int, int);
    Point findFurthestPoint();
    void findExitsAndCorners_RL();
    void findExitsAndCorners_LR();
    double distance_to_front();
    std::vector<Exit> local_Exits();
    Point closest_point();
    void findExitsAndCorners_Final();

    Detection_data detection_execution();




   // bool wallDetected(double minDistance);// Method to check if any wall is in the neighbourhood of the robot

};

#endif //detection_H
