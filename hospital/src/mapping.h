#ifndef MAPPING
#define MAPPING

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>

#include <string>
#include <sstream>
#include <cstdlib>

#include "detection.h"
#include "worldModel.h"
#include "config.h"
//#include "visualize.h"
//#include "main.pp"


typedef struct {
    double x;
    double y;
} Point_map;

typedef struct {
    double x;
    double y;
    double angle;
} Position;

typedef struct {
    Point_map point1;
    Point_map point2;
} Exit_map;

typedef struct {
    std::vector<Point_map> corners;
    Exit_map exit;
    int roomID;
    int previousRoom; //-1 if corridor
} Room;


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

void update_corners();
void update_map(int);
void update_rooms();

void update_worldModel();



Point_map local2global(Point);

std::vector<Room> map; // Just for Nazar
std::vector<Point_map> totalCorners;
std::vector<Exit_map> totalExits;

Position global_pos;
Position latest_odom;
Position odom_diff;
};
#endif // VISUALIZE


