#ifndef SM_HPP_INCLUDED
#define SM_HPP_INCLUDED

#include "worldModel.h"
#include "config.h"
#include "detection.h"

// this are the two main stages (exploring the hospital and then finding the object)
typedef enum {
    EXPLORE_HOSPITAL,
    GO_TO_ROOM,
    RETURN_TO_INIT
} High_State;

// this are the actions in the rooms
typedef enum {
    EXPLORE_CORRIDOR,   // for initial phase to count exits in the corridor
    EXIT_CORRIDOR,
    EXIT_TO_PREV_ROOM,  //-- Added-- Go to room/corridor of lower nesting level
                        //     Instead of EXIT_CORRIDOR & EXIT states
    GO_TO_START,        // after all rooms are located
    PARKING,            // park backwards

    EXPLORE_ROOM,
    GO_TO_NEXT_ROOM,
    GO_INSIDE_ROOM,      //-- Added-- Moving through the entrance/exit of a room

    STAND_NEXT_TO_OBJECT
} Low_State;

bool state_machine(struct High_state & high_st,struct Low_state & low_st, WorldModel * worldModel);

#endif
