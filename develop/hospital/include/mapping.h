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

    Mapping_data mapdata;

    Point global_pos;
    Point latest_odom;
    Point odom_diff;

    // Kept here temporarily. Remove when arrays are converted to vectors
    Detection_data localDetection;
    Exit exits_local[40];       // Data from worldmodel (written there by detection)
    Corner corners_local[40];   // Data from worldmodel (written there by detection)


    std::vector<Point> totalCorners;
    std::vector<Exit> totalExits;
    std::vector<Room> map;


    void update_global_pos ();
    void update_corners ();
    void update_rooms ();
    Point local2global(Point);

public:


    Mapping(emc::IO *io, WorldModel* worldModel);

//void update_Odometry(WorldModel*);
//void delta_Odometry(WorldModel*);


    // the only function that is called to do all the mapping
    void execute_mapping ();


};

#endif // MAPPING_H
