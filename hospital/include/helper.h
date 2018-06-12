#ifndef helper_H
#define helper_H





// Destination that is passed to the Control block
typedef struct {
    double x;
    double y;
    double angle;
    double dist;
} Destination;

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

// Define a structure to contain corridor data
// to know our location
enum Location {IN_CORRIDOR, IN_ROOM};





typedef struct {
    double x;
    double y;
} Point_map;

typedef struct {
    double x;
    double y;
    double angle;
} Position;

typedef struct {
    Point_map point1;
    Point_map point2;
} Exit_map;


// Point data
typedef struct {
    double x;
    double y;
    double angle;
    double dist;
} Point;



// Store exit data: two corners, angles and a flag whether detected
typedef struct {
    bool detected;
    Point exitPoint1;
    Point exitPoint2;
} Exit;

typedef struct  {
    int roomID;
    std::vector<Point_map> corners;
    Exit exit;
    int previousRoom;   // Corridor is 0

//    Room() {}
//    Room(Exit roomEntrance, int roomToEnterFrom) : exit(roomEntrance), previousRoom(roomToEnterFrom) {}
}Room;

typedef struct {
    bool detected;
    Point cornerPoint;
} Corner;


// Store exit data: two corners, angles and a flag whether detected
typedef struct {
    bool detected;
    Point exitPoint1_LR;
    Point exitPoint2_LR;
} Exit_LR;

typedef struct {
    bool detected;
    Point cornerPoint_LR;
} Corner_LR;

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
    std::vector<Exit_map> local_exits;
    Point closest_Point;
} Detection_data;

#endif