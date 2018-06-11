#include "worldModel.h"

/*
--------------------------------------------------------------------------------
                            Class Constructor
--------------------------------------------------------------------------------
*/

WorldModel::WorldModel() : currentLocation_(IN_CORRIDOR) { };

/*
--------------------------------------------------------------------------------
                            Private Methods
--------------------------------------------------------------------------------
*/


void WorldModel::writeJsonFile() {

    std::ofstream wfHandle(JSON_PATH);

    // Check if file has been opened
    assert(wfHandle != NULL);

    wfHandle << std::setw(4) << jsonObject_ << std::endl;
    wfHandle.close();

    jsonObject_.clear();

    std::cout << "Writing data to JSON file" << std::endl;
}

void WorldModel::readJsonFile() {

    if (!jsonObject_.empty()) {
        std::cout << "[Read/Write Error]: jsonObject_ not empty. Cannot read before writing." << std::endl;
        std::cout << "Exiting the program!" << '\n';
        exit(EXIT_FAILURE);
    }

    std::ifstream rfHandle(JSON_PATH);

    // Check if file has been opened
    assert(rfHandle != NULL);

    rfHandle >> jsonObject_;
    rfHandle.close();
}

void WorldModel::createJson() {

    for (auto i : rooms)
    {
      json r =  {
                    {"Room_ID", i.roomID},
                    {"Corners", {
                                    {"0",   {
                                                {"x", i.corners[0].x},
                                                {"y", i.corners[0].y}
                                            }
                                    },
                                    {"1",   {
                                                {"x", i.corners[1].x},
                                                {"y", i.corners[1].y}
                                            }
                                    },
                                    {"2",   {
                                                {"x", i.corners[2].x},
                                                {"y", i.corners[2].y}
                                            }
                                    },
                                    {"3",   {
                                                {"x", i.corners[3].x},
                                                {"y", i.corners[3].y}
                                            }
                                    }
                                }
                    },
                    {"Exit",    {
                                    {"0",   {
                                                {"x", i.exit.exitPoint1.x},
                                                {"y", i.exit.exitPoint1.y}
                                            }
                                    },
                                    {"1",   {
                                                {"x", i.exit.exitPoint2.x},
                                                {"y", i.exit.exitPoint2.y}
                                            }
                                    }
                                }
                    },
                    {"Previous_Room", i.previousRoom}
                };
      jsonObject_.push_back(r);
    }

};

void WorldModel::extractJson() {

    // Come up with some fancy way of extracting information from JSON object
}
/*
--------------------------------------------------------------------------------
                            Public Methods
--------------------------------------------------------------------------------
*/

/*
                ------------------------------------
                            GET METHODS
                ------------------------------------
*/

Point WorldModel::get_closestPointWall () {
    return closestPointWall_;
};

Point WorldModel::get_globalPosition () {
    return globalPosition_;
};

Point get_pointStraightAhead () {
    return pointStraightAhead_;
};

Destination get_destination () {
    return destination_;
};

Detection_data get_localDetection () {
    return localDetection_;
};

int get_enteredRooms () {
    return enteredRooms_;
};

int get_nestedExits () {
    return nestedExits_;
};

int get_currentRoom () {
    return currentRoom_;
};

int get_roomsFound () {
    return roomsFound_;
};

Location WorldModel::get_currentLocation() {
    return currentLocation_;
};

High_State get_currentHighState() {
    return currentHighState_;
};

Low_State get_currentLowState() {
    return currentLowState_;
};

std::vector<Room> get_globalRooms() {
    return globalRooms_;
};

std::vector<int> get_explorationStack() {
    return explorationStack_;
};

std::vector<int> get_connectedRooms (int baseRoom) {
    /* Write this method */
};

std::vector<Exit> getAllDetectedExits();

/*
                ------------------------------------
                            SET METHODS
                ------------------------------------
*/

void WorldModel::set_closestPointWall (Point updatedClosestPointWall) {
    closestPointWall_ = updatedClosestPointWall;
};

void WorldModel::set_globalPosition (Point updatedGlobalPosition) {
    globalPosition_ = updatedGlobalPosition;
};

void WorldModel::set_pointStraightAhead (Point updatedPointStraightAhead) {
    pointStraightAhead_ = updatedPointStraightAhead;
};

void WorldModel::set_destination (Destination updatedDestination) {
    destination_ = updatedDestination;
};

void WorldModel::set_localDetection (Detection_data updatedLocalDetection) {
    localDetection_ = updatedLocalDetection;
};

void WorldModel::set_enteredRooms (bool newRoomEntered) {
    if (newRoomEntered) {
        enteredRooms_++;
    };
};

void WorldModel::set_nestedExits (bool newNestedExitFound) {
    if (newNestedExitFound) {
        nestedExits_++;
    };
};

void WorldModel::set_currentRoom (int updatedCurrentRoom) {
    currentRoom_ = updatedCurrentRoom;
};

void WorldModel::set_roomsFound (bool newRoomFound) {
    if (newRoomFound) {
        roomsFound_++;
    };
};

void WorldModel::set_currentLocation (Location updatedLocation) {
    currentLocation_ = updatedLocation;
};

void WorldModel::set_currentHighState (High_State updatedCurrentHighState) {
    currentHighState_ = updatedCurrentHighState;
};

void WorldModel::set_currentLowState (Low_State updatedCurrentLowState) {
    currentLowState_ = updatedCurrentLowState;
};

void WorldModel::set_globalRooms (Room newRoomData) {
    globalRooms_.push_back(newRoomData);
};

void WorldModel::set_explorationStack (int newRoomToBeExplored) {
    explorationStack_.push_back(newRoomToBeExplored);
};

void WorldModel::setAllDetectedExits();
