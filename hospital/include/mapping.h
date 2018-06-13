#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>

#include <vector>

#include "worldModel.h"
#include "helper.h"
#include "config.h"

#ifndef MAPPING
#define MAPPING



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
Mapping_data execute_mapping();



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
