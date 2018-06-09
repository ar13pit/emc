#include "config.h"
#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>

#include "detection.h"
#include "planning.h"
#include "main.hpp"
#include "json.hpp"

#ifndef worldModel_H
#define worldModel_H

using json = nlohmann::json;

typedef struct {
    Point_det corners[4];
    Exit exit;
    int previousRoom;   // Corridor is 0
} Room;

// this is based on an assumption that corridor has most of the rooms connected
// we first explore the corridor (assume it is straight, any T-junctions are rooms)
// typedef struct {
//     std::vector<Exit> exits;
// } CCorridor;    //name has to be changed (conflicting class in planning for escape room challenge)


// to know our location
enum Location {IN_CORRIDOR, IN_ROOM};


class WorldModel {

    // double minDistance_;
    Point_det closestPointWall_;
    Point_det destination_;
    int enteredRooms_;
    int nestedExits_;
    Location currentLocation_;
    High_State current_high_state;
    Low_State current_low_state;
    json jsonObject_;

    void writeJson();
    void readJson();


public:
    WorldModel() : currentLocation_(IN_CORRIDOR) { };

    emc::LaserData get_laser();
    Point_det get_closestPointWall();
    Point_det get_destination();
    int get_enteredRooms();
    int get_nestedExits();
    Location get_currentLocation();
    High_State get_current_high_state();
    Low_State get_current_low_state();

    void set_destination(Destination dest);

};

#endif //worldModel_H
