#include <emc/io.h>
#include <cmath>
#include <iostream>
#include "config.h"
#include "main.hpp"

#ifndef detection_H
#define detection_H


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


// All passed data together
typedef struct {
    Exit exit;
    CorridorWalls corridor;
    Point_det furthest_point;
} Detection_data;




class Detection{
private:
    emc::IO *inOut;
    emc::LaserData laser_;
    Detection_data data_;
    Point_det LatestLaserScan[1000-2*15]; //Deleted first and last 15 Point_dets

    bool getSensorData(); // Method to obtain the sensordata
    void filterLRFData(int nFilterPoint_dets); // Filter data by sensor measurement
    void saveLRFScan();
    bool lineFit(double&, double&, int, int);

    void findCorridorWalls();
    void findFurthestPoint_det();
    void findExit();

public:
    Detection(emc::IO *io, Flags *flags) {
        inOut = io;

        if (flags->in_corridor){
            //CorridorWalls cor;
            //data->corridor = Detection::findCorridorWalls();
            findCorridorWalls();
        } else {
            //Exit *ex;
            //Point_det far;
            findExit();
            findFurthestPoint_det();
        }

    };

    Detection_data get_Detection_data();



   // bool wallDetected(double minDistance);// Method to check if any wall is in the neighbourhood of the robot




};

// the main logic function
// void detection_general(Detection_data *data, Flags *flags);

#endif //detection_H
