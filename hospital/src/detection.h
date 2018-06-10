#include <emc/io.h>
#include <emc/rate.h>
#include <cmath>
#include <iostream>
#include "config.h"
#include <cstdlib>
#include "stateMachine.h"

#ifndef detection_H
#define detection_H


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
    bool escaped;
    Point rightWall1;
    Point rightWall2;
    Point leftWall1;
    Point leftWall2;
} CorridorWalls;


// All passed data together
typedef struct {
    Exit exit;
    CorridorWalls corridor;
    Point furthest_point;
} Detection_data;




class Detection{
private:
    emc::IO *inOut;
    emc::LaserData laser_;
    Detection_data data_;
    Point LatestLaserScan[1000-2*15]; //Deleted first and last 15 Points
    bool read;

    void filterLRFData(int nFilterPoints); // Filter data by sensor measurement
    void saveLRFScan();
    bool lineFit(double&, double&, int, int);

    void findCorridorWalls();
    void findFurthestPoint();
    bool findExit();

public:
    Detection(emc::Rate *r, emc::IO *io, Flags *flags) {
        inOut = io;

//        std::cout << "read Laser Data "<< temp << std::endl;

        read = get_data(r);
        saveLRFScan();

        if (flags->in_corridor){
            //CorridorWalls cor;
            //data->corridor = Detection::findCorridorWalls();

            findCorridorWalls();
        } else {
            //Exit *ex;
            //Point far;

            findExit();
            findFurthestPoint();

//            std::cout << "Exit detected in the Detection class "<< data_.exit.detected << std::endl << std::endl;
        }
    };

    bool get_data(emc::Rate *r); // Method to obtain the sensordata
    Detection_data get_Detection_data();



   // bool wallDetected(double minDistance);// Method to check if any wall is in the neighbourhood of the robot




};

// the main logic function
// void detection_general(Detection_data *data, Flags *flags);

#endif //detection_H
