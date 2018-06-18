#ifndef helper_H
#define helper_H

#include <iostream>
#include <vector>


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

typedef struct {
    bool detected;
    Point cornerPoint;
} Corner;

typedef struct {
   bool escaped;
   Point rightWall1;
   Point rightWall2;
   Point leftWall1;
   Point leftWall2;
} CorridorWalls;


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
        Planning
------------------------------------
*/


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

// to know our location
enum Location {IN_CORRIDOR, IN_ROOM};


/*
----------------------------
        NEW SHIT!
----------------------------
*/

typedef struct {
    Exit Exits_total[40];
    Corner Corners_total[40];
    std::vector<Exit> local_exits;
    Point closest_Point;
} Detection_data;


#endif
