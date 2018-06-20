#include <iostream>
#include <vector>

#ifndef helper_H
#define helper_H


/*
---------------------------------------
        Detection
---------------------------------------
*/

// Point data
class Point {
    double angle_;
    double dist_;

public:
    double x;
    double y;

    Point ();

    // GET METHODS
    double angle();
    double dist();

    // SET METHODS
    void angle(double newAngle);
    void dist(double newDistance);
};


// Store exit data: two corners, angles and a flag whether detected
class Exit {
    bool detected_;

public:
    Point exitPoint1;
    Point exitPoint2;

    Exit ();

    // GET METHODS
    bool detected();

    // SET METHODS
    void detected(bool exitDetectedFlag);

};

class Corner {
    bool detected_;

public:
    Point cornerPoint;

    Corner();

    // GET METHODS
    bool detected();

    // SET METHODS
    void detected(bool exitDetectedFlag);

};

typedef struct {
   bool escaped;
   Point rightWall1;
   Point rightWall2;
   Point leftWall1;
   Point leftWall2;
} CorridorWalls;

typedef struct {
    Exit Exits_total[40];
    Corner Corners_total[40];
    std::vector<Exit> local_exits;
    Point closest_Point;
} Detection_data;

/*
---------------------------------------
        Mapping
---------------------------------------
*/


typedef struct  {
    int roomID;
    std::vector<Point> corners;
    Exit exit;
    int previousRoom;   // Corridor is 0

//    Room() {}
//    Room(Exit roomEntrance, int roomToEnterFrom) : exit(roomEntrance), previousRoom(roomToEnterFrom) {}
} Room;


/*
------------------------------------
        Mapping
------------------------------------
*/

typedef struct {
    Point pico_position;
    std::vector<Room> map;
} Mapping_data;


/*
--------------------------------------
            State Machine
--------------------------------------
*/

// this are the two main stages (exploring the hospital and then finding the object)
enum High_State {
    EXPLORE_HOSPITAL,
    GO_TO_ROOM,
    RETURN_TO_INIT
};

// this are the actions in the rooms
enum Low_State {
    EXPLORE_CORRIDOR,   // for initial phase to count exits in the corridor
    // EXIT_CORRIDOR,      // for the second part of the challenge as initial state

    EXIT_TO_PREV_ROOM,  // Go to room/corridor of lower nesting level


    EXPLORE_ROOM,
    GO_TO_NEXT_ROOM,     // Go to a room from the corridor or another room
    GO_INSIDE_ROOM,      // Moving through the entrance/exit of a room

    GO_TO_START,        // after all rooms are located
    PARKING,            // park backwards
    STAND_NEXT_TO_OBJECT
};

// to know our location
enum Location {IN_CORRIDOR, IN_ROOM};



#endif
