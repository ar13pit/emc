#ifndef detection_H
#define detection_H

#include <emc/io.h>
#include <emc/rate.h>
#include <cmath>
#include <iostream>

// Point data
typedef struct {
    double x;
    double y;
    double angle;
    double dist;
} Point;


// Store exit data: two corners, angles and a flag whether detected
typedef struct {
    bool detected;
    Point exitPoint1;
    Point exitPoint2;
} Exit;

typedef struct {
    bool detected;
    Point cornerPoint;
} Corner;


// Store exit data: two corners, angles and a flag whether detected
typedef struct {
    bool detected;
    Point exitPoint1_LR;
    Point exitPoint2_LR;
} Exit_LR;

typedef struct {
    bool detected;
    Point cornerPoint_LR;
} Corner_LR;

typedef struct {
    bool escaped;
    Point rightWall1;
    Point rightWall2;
    Point leftWall1;
    Point leftWall2;
} CorridorWalls;

typedef struct {
    Point exitPoint1;
    Point exitPoint2;
    Point exitPoint1_LR;
    Point exitPoint2_LR;
    Point cornerPoint;
    Point cornerPoint_LR;
} Detection_data;

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
    Exit_LR Exits_LR[20];
    Corner_LR Corners_LR[20];
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




   // bool wallDetected(double minDistance);// Method to check if any wall is in the neighbourhood of the robot

};

#endif //detection_H
