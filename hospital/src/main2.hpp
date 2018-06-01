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
    GO_TO_ROOM
} High_State;

// this are the actions in the rooms
typedef enum {
    EXPLORE_ROOM,
    GO_TO_NEXT_ROOM,
    EXIT,
    GO_TILL_FACED_THE_WALL //for initial phase to count exits in the corridor
} Low_State;

// to know our location
typedef struct {
    bool in_room;
    bool in_corridor;
} Location;


#endif
