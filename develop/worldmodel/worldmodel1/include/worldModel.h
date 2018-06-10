#include <iostream>
#include <fstream>
#include <vector>

#include "config.h"

#include "detection.h"
#include "planning.h"
#include "main.hpp"
#include "json.hpp"

#ifndef worldModel_H
#define worldModel_H

using json = nlohmann::json;

struct Room {
    int roomID;
    Point corners[4];
    Exit exit;
    int previousRoom;   // Corridor is 0

    Room() {};
    Room(Exit roomEntrance, int roomToEnterFrom) : exit(roomEntrance), previousRoom(roomToEnterFrom) {};
} ;

typedef struct Room Room;

// Define a structure to contain corridor data

// to know our location
enum Location {IN_CORRIDOR, IN_ROOM};


class WorldModel {

    std::vector<Room> globalRooms_;
    std::vector<int> explorationStack_;     // Contains list of room numbers that robot must go to

    Point closestPointWall_;
    Point globalPosition_;
    Point pointStraightAhead_;

    Destination destination_;

    int enteredRooms_;
    int nestedExits_;
    int currentRoom_;
    int roomsFound_;

    Location currentLocation_;

    High_State current_high_state;
    Low_State current_low_state;

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

    int get_enteredRooms();
    int get_nestedExits();
    int get_currentRoom();                          // Renamed from     int getCurrentRoom();
    int get_roomsFound();

    Location get_currentLocation();

    High_State get_current_high_state();
    Low_State get_current_low_state();

    std::vector<Room> get_globalRooms();            // Renamed from     std::vector<Room> getAllRooms();
    std::vector<int> get_explorationStack();


    // Set Methods
    void set_globalPosition(Point updatedGlobalPosition);
    void set_currentLocation(Location newLocation);
    void set_destination(Destination dest);

};

#endif //worldModel_H
