#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "config.h"
#include "json.hpp"
#include "helper.h"

#ifndef worldModel_H
#define worldModel_H


/*
 * initialize int variables
 * assign the variables from the detection_data to the corresponding variables in the private functions
 * */

using json = nlohmann::json;




class WorldModel {

    std::vector<Room> globalRooms_;
    std::vector<int> explorationStack_;     // Contains list of room numbers that robot must go to

    std::vector<Exit> allDetectedExits_;
    std::vector<Point> allDetectedCorners_;

    Point closestPointWall_;
    Point globalPosition_;
    Point pointStraightAhead_;
    Point destination_;

    Detection_data localDetection_;
    Mapping_data currentMappingData_;

    int enteredRooms_;
    int nestedExits_;
    int currentRoomID_;
    int roomsFound_;

    Location currentLocation_;

    High_State currentHighState_;
    Low_State currentLowState_;

    Room mostNestedRoom_;
    Room closestRoom_;
    Room currentRoom_;
    Room nextRoom_;

    json jsonObject_;

    // Private Methods
    void writeJsonFile ();
    void readJsonFile ();
    void createJson ();
    void extractJson ();

    void set_allDetectedExits (std::vector<Exit> allDetectedExits);
    void set_variablesRelatedToDetectionData ();
    void set_updateMappingVariables ();
    void set_currentRoomID ();


public:
    // Class Constructor
    WorldModel();

    // Get Methods
    Point get_closestPointWall();
    Point get_globalPosition();                     // Renamed from     Point getCurrentPosition();
    Point get_pointStraightAhead();

    Point get_destination();

    Detection_data get_localDetection();

    int get_enteredRooms();
    int get_nestedExits();
    int get_currentRoomID();                          // Renamed from     int getCurrentRoom();
    int get_roomsFound();               // how many rooms are found

    Room get_mostNestedRoom();
    Room get_closestRoom();
    Room get_currentRoom();
    Room get_nextRoom();

    Location get_currentLocation();

    High_State get_currentHighState();              // Renamed from     High_State get_current_high_state();
    Low_State get_currentLowState();                // Renamed from     Low_State get_current_low_state();

    std::vector<Room> get_globalRooms();            // Renamed from     std::vector<Room> getAllRooms();
    std::vector<int> get_explorationStack();
    std::vector<int> get_connectedRooms(int baseRoom);          // Never call this method during an ongoing exploration of any room.
    std::vector<Exit> get_allDetectedExits();
    std::vector<Point> get_allDetectedCorners();

    Mapping_data get_mapping();


    // Set Methods
    void set_closestPointWall(Point updatedClosestPointWall);
    void set_globalPosition(Point updatedGlobalPosition);
    void set_pointStraightAhead(Point updatedPointStraightAhead);

    void set_destination(Point updatedDestination);

    void set_localDetection(Detection_data updatedLocalDetection);

    void set_enteredRooms(bool newRoomEntered);
    void set_nestedExits(bool newNestedExitFound);
    void set_roomsFound(bool newRoomFound);         // add a new room

    void set_currentLocation(Location updatedLocation);

    void set_currentHighState(High_State updatedCurrentHighState);
    void set_currentLowState(Low_State updatedCurrentLowState);

    void set_globalRooms(Room newRoomData);
    void set_explorationStack(int newRoomToBeExplored);

    void set_mostNestedRoom();
    void set_closestRoom();
    void set_currentRoom(Room room);
    void set_nextRoom();
    void set_mapping(Mapping_data updateMappingData);

    void set_allDetectedCorners(std::vector<Point> allDetectedCorners_);


    // Other Methods (sorry)
    Room findRoomByRoomNumber(int roomNumber);

    // Check Methods
    bool check_roomExists(Exit exitDataInGlobalCoordinates);    // Returns false if room with this input exit (in GC) exists

    // IO to JSON
    void update_JSON();
    void update_roomInGlobalRooms(Room updatedRoomData);
};

#endif //worldModel_H
