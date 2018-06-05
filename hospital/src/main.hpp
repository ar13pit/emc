#ifndef MAIN_HPP
#define MAIN_HPP


typedef struct {
    bool in_corridor;   // check the phase of the escape
    bool turned_once;   // in the room when turned once
    bool escaped;       // finished
    bool drive_frw;
    bool turn;          // should be active when turning 180
} Flags;

// this are the two main stages (exploring the hospital and then finding the object)
typedef enum {
    EXPLORE_HOSPITAL,
    GO_TO_ROOM,
    RETURN_TO_INIT,
    TOO_CLOSE_TO_WALL
} High_State;

// this are the actions in the rooms
typedef enum {
    EXPLORE_CORRIDOR,   // for initial phase to count exits in the corridor
    EXIT_CORRIDOR,
    GO_TO_START,        // after all rooms are located
    PARKING,            // park backwards

    EXPLORE_ROOM,
    GO_TO_NEXT_ROOM,
    EXIT,
    EXIT_MAIN_ROOM,      // when returning from the nested room to the main room, don't explore it

    STAND_NEXT_TO_OBJECT
} Low_State;




#endif
