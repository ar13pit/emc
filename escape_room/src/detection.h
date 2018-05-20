#ifndef detection_H
#define detection_H

#include <emc/io.h>
#include <cmath>
#include <iostream>

// the main logic function
void detection_general(Detection_data *data, bool in_corridor);


// All passed data together
typedef struct {
    Exit exit;
    CorridorWalls corridor;
    Point_det furthest_point;
} Detection_data;


// Point_det data
typedef struct {
    double x;
    double y;
    double angle;
    double dist;
} Point_det;


// Store exit data: two corners, angles and a flag whether detected
typedef struct {
    bool detected;
    Point_det exitPoint_det1;
    Point_det exitPoint_det2;
} Exit;

typedef struct {
    bool escaped;
    Point_det rightWall1;
    Point_det rightWall2;
    Point_det leftWall1;
    Point_det leftWall2;
} CorridorWalls;


class Detection{
private:
    emc::IO *inOut;

public:
 Detection(emc::IO *io){
        inOut = io;
        laser = emc::LaserData();

        return;
    }

    Point_det LatestLaserScan[1000-2*15]; //Deleted first and last 15 Point_dets
    emc::LaserData laser;
    CorridorWalls findCorridorWalls();
    bool getSensorData(); // Method to obtain the sensordata
    void filterLRFData(emc::LaserData* laser,int nFilterPoint_dets); // Filter data by sensor measurement
    void saveLRFScan(emc::LaserData* laser);
    bool lineFit(double&, double&, int, int);
    Point_det findFurthestPoint_det();
    Exit findExit();

   // bool wallDetected(double minDistance);// Method to check if any wall is in the neighbourhood of the robot




};

#endif //detection_H
