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
    // assert(wfHandle != NULL);

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
    // assert(rfHandle != NULL);

    rfHandle >> jsonObject_;
    rfHandle.close();
}

void WorldModel::createJson() {

    for (auto i : rooms)
    {
      json room, corners, exits;
      room.emplace("Room_ID", i.roomID);

      int cornerID = 0;

      for (auto j: i.corners) {

          json c = { { "x", j.x }, { "y", j.y } } ;
          std::cout << "Room_ID: " << i.roomID << ", Corner_ID: " << cornerID << ", Points: " << c << '\n';
          corners.emplace(std::to_string(cornerID), c);
          cornerID++;

      }

      room.emplace("Corners", corners);

      json ep1 = { { "x", i.exit.exitPoint1.x }, { "y", i.exit.exitPoint1.y } } ;
      json ep2 = { { "x", i.exit.exitPoint2.x }, { "y", i.exit.exitPoint2.y } } ;
      exits.emplace("0", ep1);
      exits.emplace("1", ep2);

      room.emplace("Exit", exits);

      room.emplace("Previous_Room", i.previousRoom);

      jsonObject_.push_back(room);
    }

};

void WorldModel::extractJson() {

    assert(jsonObject_.is_array());
    for (const auto& c : jsonObject_)
    {
      Room tempRoom;
      Exit tempExit;
      Point tempPoint;
      std::vector<Point> tempCorners;


      assert(c.is_object());

      assert(c.find("Room_ID") != c.end());
      assert(c.find("Previous_Room") != c.end());
      assert(c.find("Exit") != c.end());
      assert(c.find("Corners") != c.end());

      tempRoom.roomID = c["Room_ID"];
      tempRoom.previousRoom = c["Previous_Room"];

      json corner = c["Corners"];

      for (json::iterator it = corner.begin(); it != corner.end(); ++it) {

        json pt = it.value();         // it.key() returns the key (which is a str object)
        Point cornerPoint;

        cornerPoint.x = pt["x"];
        cornerPoint.y = pt["y"];
        tempCorners.push_back(cornerPoint);
      }

      tempRoom.corners = tempCorners;

      tempPoint.x = c["Exit"]["0"]["x"];
      tempPoint.y = c["Exit"]["0"]["y"];
      tempExit.exitPoint1 = tempPoint;

      tempPoint.x = c["Exit"]["1"]["x"];
      tempPoint.y = c["Exit"]["1"]["y"];
      tempExit.exitPoint2 = tempPoint;

      tempRoom.exit = tempExit;

      std::cout << "----------------------------------------------------------------------------" << '\n';
      std::cout << "Room_ID: " << tempRoom.roomID << ", Previous_Room: " << tempRoom.previousRoom << '\n';

      int cornerID = 0;
      for (auto corner_it : tempRoom.corners) {
          std::cout << "Corner_ID: " << cornerID << "(" << corner_it.x << "," << corner_it.y << ")" << '\n';
          cornerID++;
      }

      std::cout << "Exit 1: (" << tempRoom.exit.exitPoint1.x << "," << tempRoom.exit.exitPoint1.y << ")"  << '\n';
      std::cout << "Exit 2: (" << tempRoom.exit.exitPoint2.x << "," << tempRoom.exit.exitPoint2.y << ")"  << '\n';
      std::cout << "----------------------------------------------------------------------------" << '\n';


    }


    std::cout << "Done!" << std::endl;

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

Point WorldModel::get_pointStraightAhead () {
    return pointStraightAhead_;
};

Destination WorldModel::get_destination () {
    return destination_;
};

Detection_data WorldModel::get_localDetection () {
    return localDetection_;
};

int WorldModel::get_enteredRooms () {
    return enteredRooms_;
};

int WorldModel::get_nestedExits () {
    return nestedExits_;
};

int WorldModel::get_currentRoom () {
    return currentRoom_;
};

int WorldModel::get_roomsFound () {
    return roomsFound_;
};

Location WorldModel::get_currentLocation() {
    return currentLocation_;
};

High_State WorldModel::get_currentHighState() {
    return currentHighState_;
};

Low_State WorldModel::get_currentLowState() {
    return currentLowState_;
};

std::vector<Room> WorldModel::get_globalRooms() {
    return globalRooms_;
};

std::vector<int> WorldModel::get_explorationStack() {
    return explorationStack_;
};

std::vector<int> WorldModel::get_connectedRooms (int baseRoom) {
    /* Write this method */
    std::vector<int> connectedRooms;

    return connectedRooms;
};

// std::vector<Exit> getAllDetectedExits() {};

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

// void WorldModel::setAllDetectedExits();
