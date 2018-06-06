#include "config.h"
#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>

#include "detection.h"
#include "planning.h"
#include "main.hpp"

#ifndef worldModel_H
#define worldModel_H

using json = nlohmann::json;

typedef struct {
    std::vector<Point_det> corners;
    std::vector<Exit> exits;
} Room;


// this is based on an assumption that corridor has most of the rooms connected
// we first explore the corridor (assume it is straight, any T-junctions are rooms)
typedef struct {
    std::vector<Exit> exits;
} CCorridor;    //name has to be changed (conflicting class in planning for escape room challenge)


// to know our location
// struct Location {
//     bool in_room;
//     bool in_nested_room;
//     bool in_corridor;
// } ;

typedef enum Location {
    IN_ROOM;
    IN_NESTED_ROOM;
    IN_CORRIDOR;
  };

class WorldModel {
    emc::IO* io_;
    emc::rate r_;
    emc::LaserData laser_;

    // double minDistance_;
    Point_det closestPointWall_;
    Point_det destination_;
    int enteredRooms_;
    int nestedExits_;
    Location currentLocation_;
    json jsonObject_;

    void writeJson();
    void readJson();


public:
WorldModel(emc::IO* IO) : io_(IO), r_(EXECUTION_RATE) {
        // minDistance_ = MAX_RANGE_LRF;
    }

    // double* getMinimumDistance();
    // void setMinimumDistance(emc::LaserData* laser); // Method to determine the minimum distance to a wall
    emc::LaserData get_laser();
    Point_det get_closestPointWall();
    Point_det get_destination();
    int get_enteredRooms();
    int get_nestedExits();
    Location get_currentLocation();
};

#endif //worldModel_H
