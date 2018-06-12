#include <fstream>
#include <cassert>
#include <iostream>
// #include <string>
#include "./json.hpp"
// #include "./corner.h"

using json = nlohmann::json;

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point exitPoint1;
    Point exitPoint2;
} Exit;

struct Room {
    int roomID;
    std::vector<Point> corners;
    Exit exit;
    int previousRoom;   // Corridor is 0

    Room() {};
    Room(Exit roomEntrance, int roomToEnterFrom) : exit(roomEntrance), previousRoom(roomToEnterFrom) {};
};

int main()
{
  // read the json object from a file
  std::ifstream inFile("rooms.json");
  json jsonObject_;
  inFile >> jsonObject_;

  std::vector<Room> rooms;

  // copy the data from a json object
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
