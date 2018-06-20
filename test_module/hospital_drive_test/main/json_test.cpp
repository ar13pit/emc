// #include <emc/io.h>
// #include <emc/rate.h>
// #include <emc/odom.h>
// #include <cmath>
#include <iostream>
// #include <cstdlib>
#include <string>


#include "worldModel.hpp"
// #include "config.hpp"
#include "helper.hpp"
// #include "visualize.hpp"


int main ()
{
    WorldModel worldModel;
    Room newRoom, updatedRoom;

    newRoom.roomID = 0;
    newRoom.previousRoom = -1;

    worldModel.set_globalRooms(newRoom);

    worldModel.update_JSON();

    // worldModel.update_roomInGlobalRooms(updatedRoom);

    return 0;
}
