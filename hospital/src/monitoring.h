#ifndef MONITORS_HPP_INCLUDED
#define MONITORS_HPP_INCLUDED

#include "main.hpp"
#include "worldModel.h"

// detection part
bool detection();           // detecting corners method
bool interpreteation();     // constructing room and corridor objects
bool coordinate_trasf();    // transform the coordinated of the room to save in JSON file


// planning part
bool wall_push_back();      // low-level planning to move away from the wall
bool corridor_end();        // move to the end of the corridor
bool exit_corridor();       // exit corridor (refer to the last exit)
bool explore_room();        // turning and scanning (stop command or moving)
bool go_to_next_room();
bool exit();
bool exit_main_room();

// unite the program execution
bool monitoring(WorldModel *worldModel);
bool explore_hospital(WorldModel *worldModel);
bool return_to_init(WorldModel * worldModel);

#endif
/*
EXPLORE_CORRIDOR,   // for initial phase to count exits in the corridor
EXIT_CORRIDOR,
GO_TO_START,        // after all rooms are located
PARKING,            // park backwards

EXPLORE_ROOM,
GO_TO_NEXT_ROOM,
EXIT,
EXIT_MAIN_ROOM,      // when returning from the nested room to the main room, don't explore it

STAND_NEXT_TO_OBJECT */
