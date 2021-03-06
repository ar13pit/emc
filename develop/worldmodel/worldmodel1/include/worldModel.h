#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "config.h"
#include "detection.h"
#include "planning.h"
#include "stateMachine.h"
// #include "worldModelDataTypes.h"
#include "json.hpp"

#ifndef worldModel_H
#define worldModel_H


using json = nlohmann::json;

// Define a structure to contain corridor data

// to know our location


class WorldModel {

    std::vector<Room> globalRooms_;
    std::vector<int> explorationStack_;     // Contains list of room numbers that robot must go to

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
    int get_roomsFound();

    Location get_currentLocation();

    High_State get_currentHighState();              // Renamed from     High_State get_current_high_state();
    Low_State get_currentLowState();                // Renamed from     Low_State get_current_low_state();

    std::vector<Room> get_globalRooms();            // Renamed from     std::vector<Room> getAllRooms();
    std::vector<int> get_explorationStack();
    std::vector<int> get_connectedRooms(int baseRoom);          // Never call this method during an ongoing exploration of any room.
    // std::vector<Exit> getAllDetectedExits();


    // Set Methods
    void set_closestPointWall(Point updatedClosestPointWall);
    void set_globalPosition(Point updatedGlobalPosition);
    void set_pointStraightAhead(Point updatedPointStraightAhead);

    void set_destination(Destination updatedDestination);

    void set_localDetection(Detection_data updatedLocalDetection);

    void set_enteredRooms(bool newRoomEntered);
    void set_nestedExits(bool newNestedExitFound);
    void set_currentRoom(int updatedCurrentRoom);
    void set_roomsFound(bool newRoomFound);

    void set_currentLocation(Location updatedLocation);

    void set_currentHighState(High_State updatedCurrentHighState);
    void set_currentLowState(Low_State updatedCurrentLowState);

    void set_globalRooms(Room newRoomData);
    void set_explorationStack(int newRoomToBeExplored);
    // void setAllDetectedExits();

};

#endif //worldModel_H
