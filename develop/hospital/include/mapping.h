#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "worldModel.h"
#include "helper.h"
#include "config.h"

#ifndef MAPPING_H
#define MAPPING_H



class Mapping {

    emc::IO *inOut;
    emc::OdometryData odom;
    WorldModel* WM;

public:
    std::vector<Room> map;
    std::vector<Exit_map> totalExits;
    std::vector<Point_map> totalCorners;

    Point global_pos;
    Point latest_odom;
    Point odom_diff;

    Mapping(emc::IO *io, WorldModel* worldModel);

    void init_map ();
    void update_global_pos ();
//void update_Odometry(WorldModel*);
//void delta_Odometry(WorldModel*);

    void update_corners ();
    void update_rooms ();
    void execute_mapping ();

// the only function that is called to do all the mapping
    void update_map();

    Point local2global(Point);

};

#endif // VISUALIZE
