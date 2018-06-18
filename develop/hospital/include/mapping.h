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
    Mapping(emc::IO *io);

void init_map(WorldModel*);
void update_global_pos(WorldModel*);
//void update_Odometry(WorldModel*);
//void delta_Odometry(WorldModel*);

void update_corners(WorldModel*);
void update_rooms(WorldModel*);
void execute_mapping(WorldModel * worldModel);



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
