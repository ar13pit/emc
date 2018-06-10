#include "worldmodel.h"

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

    // int roomID;
    // Point corners[4];
    // Exit exit;
    // int previousRoom;   // Corridor is 0

    for (auto i : rooms)
    {
      json r = { {"Room_ID", i.roomID}, {"Corners", {} },{ "x", i.x_ }, { "y", i.y_ } };
      jsonObject_.push_back(r);
    }

};

/*
--------------------------------------------------------------------------------
                            Public Methods
--------------------------------------------------------------------------------
*/

Location WorldModel::get_currentLocation() {
    return currentLocation_;
};

void WorldModel::set_currentLocation(Location newLocation) {
    currentLocation_ = newLocation;
};

Point WorldModel::get_globalPosition() {
    return globalPosition_;
};

void WorldModel::set_globalPosition(Point updatedGlobalPosition) {
    globalPosition_ = updatedGlobalPosition;
};
