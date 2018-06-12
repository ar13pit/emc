#include "worldModel.h"
#include "config.h"

#include <vector>

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


//void WorldModel::writeJsonFile() {

//    std::ofstream wfHandle(JSON_PATH);

//    // Check if file has been opened
//    assert(wfHandle != NULL);

//    wfHandle << std::setw(4) << jsonObject_ << std::endl;
//    wfHandle.close();

//    jsonObject_.clear();

//    std::cout << "Writing data to JSON file" << std::endl;
//}

//void WorldModel::readJsonFile() {

//    if (!jsonObject_.empty()) {
//        std::cout << "[Read/Write Error]: jsonObject_ not empty. Cannot read before writing." << std::endl;
//        std::cout << "Exiting the program!" << '\n';
//        exit(EXIT_FAILURE);
//    }

//    std::ifstream rfHandle(JSON_PATH);

//    // Check if file has been opened
//    assert(rfHandle != NULL);

//    rfHandle >> jsonObject_;
//    rfHandle.close();
//}

//void WorldModel::createJson() {

//    for (auto i : rooms)
//    {
//      json r =  {
//                    {"Room_ID", i.roomID},
//                    {"Corners", {
//                                    {"0",   {
//                                                {"x", i.corners[0].x},
//                                                {"y", i.corners[0].y}
//                                            }
//                                    },
//                                    {"1",   {
//                                                {"x", i.corners[1].x},
//                                                {"y", i.corners[1].y}
//                                            }
//                                    },
//                                    {"2",   {
//                                                {"x", i.corners[2].x},
//                                                {"y", i.corners[2].y}
//                                            }
//                                    },
//                                    {"3",   {
//                                                {"x", i.corners[3].x},
//                                                {"y", i.corners[3].y}
//                                            }
//                                    }
//                                }
//                    },
//                    {"Exit",    {
//                                    {"0",   {
//                                                {"x", i.exit.exitPoint1.x},
//                                                {"y", i.exit.exitPoint1.y}
//                                            }
//                                    },
//                                    {"1",   {
//                                                {"x", i.exit.exitPoint2.x},
//                                                {"y", i.exit.exitPoint2.y}
//                                            }
//                                    }
//                                }
//                    },
//                    {"Previous_Room", i.previousRoom}
//                };
//      jsonObject_.push_back(r);
//    }

//};

//void WorldModel::extractJson() {

//    // Come up with some fancy way of extracting information from JSON object
//}
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

Point WorldModel::get_pointStraightAhead() {
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

std::vector<int> get_connectedRooms (int baseRoom) {
    /* Write this method */
};

Room WorldModel::get_mostNestedRoom(){
    return mostNestedRoom_;
}

Room WorldModel::get_closestRoom(){
    return closestRoom_;
}

Room WorldModel::get_curRoom(){
    return curRoom_;
}

Room WorldModel::get_nextRoom(){
    return nextRoom_;
}

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

void WorldModel::set_mostNestedRoom(){

    std::vector<Room> allRooms = get_globalRooms();
    Room mostNestedRoom;
    int highestNesting = 0;

    for(int i=0; i<allRooms.size(); i++){
        int countNesting = 0;
        Room lowerRoom = allRooms[i];

        //While previousRoom is not the corridor
        while(lowerRoom.previousRoom != 0){

            lowerRoom = findRoomByRoomNumber(lowerRoom.previousRoom);
            countNesting++;
        }
        if(countNesting > highestNesting){
            mostNestedRoom = allRooms[i];
        }
    }
    std::cout << "MostNestedRoom = Room nested in Nr " << mostNestedRoom.previousRoom << std::endl;
    mostNestedRoom_ = mostNestedRoom;
}

void WorldModel::set_closestRoom(){

    Room closestRoom;
    std::vector<Room> allRooms = get_globalRooms();
    int curRoom = get_currentRoom();
    Point curPos = get_globalPosition();
    double shortestDist = INFINITY;

    for(int i = 0;i<allRooms.size(); i++){

        if(allRooms[i].exit.detected && curRoom == allRooms[i].previousRoom &&
               closestRoom.corners.size()){
            //Get middle point of the exit

            double xMid = 0.5*(allRooms[i].exit.exitPoint1.x + allRooms[i].exit.exitPoint2.x);
            double yMid = 0.5*(allRooms[i].exit.exitPoint1.y + allRooms[i].exit.exitPoint2.y);

            double distToRoom = sqrt(pow(curPos.x-xMid, 2) + pow(curPos.y-yMid, 2));
            if(distToRoom < shortestDist){
                shortestDist = distToRoom;
                closestRoom = allRooms[i];
            }
            //cout << "Exit Room " << i << " is at (" << xMid << ',' << yMid << ')' << endl;
        }
    }
    //cout << "Closest Exit is at (" << 0.5*(closestRoom.exit_previous.exitPoint1.x + closestRoom.exit_previous.exitPoint2.x) << ',' << 0.5*(closestRoom.exit_previous.exitPoint1.y + closestRoom.exit_previous.exitPoint2.y) << ')' << endl;
    closestRoom_ = closestRoom;
}

void WorldModel::set_curRoom(Room curRoom){
    curRoom_ = curRoom;
}

void WorldModel::set_nextRoom(){

    int curRoom = get_currentRoom();
    Room nextRoom = get_mostNestedRoom();

    while(nextRoom.previousRoom != curRoom){
        nextRoom = findRoomByRoomNumber(nextRoom.previousRoom);;
    }
    nextRoom_ = nextRoom;
}


void WorldModel::setAllDetectedExits(std::vector<Exit> allDetectedExits){
    allDetectedExits_ = allDetectedExits;
}

//////////////////////// EXTRA METHODS ///////////////////////////////////
Room WorldModel::findRoomByRoomNumber(int roomNumber){

    std::vector<Room> allRooms = get_globalRooms();

    for(int i=0; i<allRooms.size(); i++){
        if(allRooms[i].roomID == roomNumber)
            return allRooms[i];
    }
}
