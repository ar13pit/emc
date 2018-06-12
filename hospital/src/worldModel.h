#include <iostream>
#include <fstream>
#include <vector>

#include "config.h"

#include "detection.h"
#include "planning.h"
#include "stateMachine.h"
#include "json.hpp"

#ifndef worldModel_H
#define worldModel_H


using json = nlohmann::json;

struct Room {
    int roomID;
    std::vector<Point> corners;
    Exit exit;
    int previousRoom;   // Corridor is 0

    Room() {};
    Room(Exit roomEntrance, int roomToEnterFrom) : exit(roomEntrance), previousRoom(roomToEnterFrom) {};
};

typedef struct Room Room;

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


class WorldModel {

    std::vector<Room> globalRooms_;
    std::vector<int> explorationStack_;     // Contains list of room numbers that robot must go to

    std::vector<Exit> allDetectedExits_;

    Point closestPointWall_;
    Point globalPosition_;
    Point pointStraightAhead_;

    Destination destination_;

    Detection_data localDetection_;

    int enteredRooms_;
    int nestedExits_;
    int currentRoom_;
    int roomsFound_;

    Location currentLocation_;

    High_State currentHighState_;
    Low_State currentLowState_;

    Room mostNestedRoom_;
    Room closestRoom_;
    Room curRoom_;
    Room nextRoom_;

    json jsonObject_;

    // Private Methods
    void writeJsonFile();
    void readJsonFile();
    void createJson();
    void extractJson();

public:
    // Class Constructor
    WorldModel();

    // Get Methods
    Point get_closestPointWall();
    Point get_globalPosition();                     // Renamed from     Point getCurrentPosition();
    Point get_pointStraightAhead();

    Destination get_destination();

    Detection_data get_localDetection();

    int get_enteredRooms();
    int get_nestedExits();
    int get_currentRoom();                          // Renamed from     int getCurrentRoom();
    int get_roomsFound();               // how many rooms are found

    Room get_mostNestedRoom();
    Room get_closestRoom();
    Room get_curRoom();
    Room get_nextRoom();

    Location get_currentLocation();

    High_State get_currentHighState();              // Renamed from     High_State get_current_high_state();
    Low_State get_currentLowState();                // Renamed from     Low_State get_current_low_state();

    std::vector<Room> get_globalRooms();            // Renamed from     std::vector<Room> getAllRooms();
    std::vector<int> get_explorationStack();
    std::vector<int> get_connectedRooms(int baseRoom);          // Never call this method during an ongoing exploration of any room.
    std::vector<Exit> getAllDetectedExits();


    // Set Methods
    void set_closestPointWall(Point updatedClosestPointWall);
    void set_globalPosition(Point updatedGlobalPosition);
    void set_pointStraightAhead(Point updatedPointStraightAhead);

    void set_destination(Destination updatedDestination);

    void set_localDetection(Detection_data updatedLocalDetection);

    void set_enteredRooms(bool newRoomEntered);
    void set_nestedExits(bool newNestedExitFound);
    void set_currentRoom(int updatedCurrentRoom);
    void set_roomsFound(bool newRoomFound);         // add a new room

    void set_currentLocation(Location updatedLocation);

    void set_currentHighState(High_State updatedCurrentHighState);
    void set_currentLowState(Low_State updatedCurrentLowState);

    void set_globalRooms(Room newRoomData);
    void set_explorationStack(int newRoomToBeExplored);

    void set_mostNestedRoom();
    void set_closestRoom();
    void set_curRoom(Room room);
    void set_nextRoom();

    void setAllDetectedExits(std::vector<Exit> allDetectedExits);

    // Other Methods (sorry)
    Room findRoomByRoomNumber(int roomNumber);

    // Check Methods
    bool check_roomExists(Exit exitDataInGlobalCoordinates);    // Returns false if room with this input exit (in GC) exists
};

#endif //worldModel_H
