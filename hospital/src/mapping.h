#ifndef MAPPING
#define MAPPING

#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>

#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>

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
void update_rooms();


// the only function that is called to do all the mapping
void update_map();


Point_map local2global(Point);

std::vector<Room> map;
std::vector<Point_map> totalCorners;
std::vector<Exit_map> totalExits;

Position global_pos;
Position latest_odom;
Position odom_diff;
};

#endif // VISUALIZE


