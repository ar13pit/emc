#ifndef WorldModelDataTypes_H
#define WorldModelDataTypes_H

// namespace mapping {
//
typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point exitPoint1;
    Point exitPoint2;
} Exit;

struct Room {
    int roomID;
    std::vector<Point> corners;
    Exit exit;
    int previousRoom;   // Corridor is 0

    Room() {};
    Room(Exit roomEntrance, int roomToEnterFrom) : exit(roomEntrance), previousRoom(roomToEnterFrom) {};
};

typedef struct Room Room;

// }

// namespace detection {

typedef struct {
    Point exitPoint1;
    Point exitPoint2;
    Point exitPoint1_LR;
    Point exitPoint2_LR;
    Point cornerPoint;
    Point cornerPoint_LR;
} Detection_data;

// }

// namespace planning {

typedef struct {
    double x;
    double y;
    double dist;
    double angle;
} Destination;

// }


// namespace statemachine {
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

enum Location {IN_CORRIDOR, IN_ROOM};


// }




#endif
