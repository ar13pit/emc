#ifndef MAPPING
#define MAPPING

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include <emc/io.h>
#include <emc/rate.h>
#include <cmath>
#include <iostream>



typedef struct {
    double x;
    double y;
    double angle;
} Position;

typedef struct {
    Point point1;
    Point point2;
} Exit_map;

typedef struct {
    std::vector<Point> corners;
    Exit_map exit;
} Room;


std::vector<Room> map; // Just for Nazar


class Mapping
{
private:
    emc::IO *inOut;
    emc::OdometryData odom;
public:
    Mapping(emc::IO *io){

        inOut = io;
        odom = emc::OdometryData();

        return;
    }

void init_map();
void update_global_pos();
void update_Odometry();
void delta_Odometry();


Position global_pos;
Position latest_odom;
Position odom_diff;
};
#endif // VISUALIZE


