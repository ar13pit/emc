#ifndef detection_H
#define detection_H

#include <emc/io.h>

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


class Detection{
private:
    emc::IO *inOut;

public:
 Detection(emc::IO *io){
        inOut = io;
        laser = emc::LaserData();

        return;
    }

    Point LatestLaserScan[1200]; //Deleted first and last 15 points
    emc::LaserData laser;
    CorridorWalls findCorridorWalls();
    bool getSensorData(); // Method to obtain the sensordata
    void filterLRFData(int nFilterPoints); // Filter data by sensor measurement
    void saveLRFScan();
    bool lineFit(double&, double&, int, int);
    Point findFurthestPoint();
    Exit findExit();

   // bool wallDetected(double minDistance);// Method to check if any wall is in the neighbourhood of the robot




};

#endif //detection_H
