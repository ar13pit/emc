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


void WorldModel::writeJson() {
    for (auto i : rooms)
    {
      json r = { { "x", corner.x_ }, { "y", corner.y_ } };
      jsonObject_.push_back(c);
    }

    std::ofstream wfHandle(JSON_PATH, ios::out | ios::app);
    wfHandle << std::setw(4) << jsonObject_ << std::endl;
    wfHandle.close();

    // Clear jsonObject_

    std::cout << "Writing data to JSON file" << std::endl;
}

void WorldModel::readJson() {
    std::ifstream rfHandle(JSON_PATH);


}


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
