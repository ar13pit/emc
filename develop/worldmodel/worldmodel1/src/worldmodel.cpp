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
