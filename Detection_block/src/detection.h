#ifndef detection_H
#define detection_H

#include <emc/io.h>

// Furthest point
typedef struct {
    double x;
    double y;
    double angle;
    double dist;
} Furthest_point;


// Store exit data: two corners, angles and a flag whether detected
typedef struct {
    bool detected;
    double x1;
    double y1;
    double x2;
    double y2;
    double angle1;
    double angle2;
} Exit;

// Point data
typedef struct {
    double x;
    double y;
    double angle;
    double dist;
} Point;


class Detection{
private:
    emc::IO *inOut;

public:
 Detection(emc::IO *io){
        inOut = io;
        laser = emc::LaserData();

        return;
    }

    Point LatestLaserScan[1000-2*15]; //Deleted first and last 15 points
    emc::LaserData laser;
    bool getSensorData(); // Method to obtain the sensordata
    void filterLRFData(emc::LaserData* laser,int nFilterPoints); // Filter data by sensor measurement
    void saveLRFScan(emc::LaserData* laser);
   // bool wallDetected(double minDistance);// Method to check if any wall is in the neighbourhood of the robot


};

#endif //detection_H
