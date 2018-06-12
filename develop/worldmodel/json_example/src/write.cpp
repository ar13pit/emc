#include <vector>
#include <iostream>
#include <fstream>
#include "./json.hpp"
// #include "./corner.h"
#include <string>

using json = nlohmann::json;

// typedef struct {
//     int roomID;
//     std::vector<Corner> corners;
//     Exit exit;
//     int previousRoom;
// } Room;

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

//typedef struct Room Room;


int main()
{
  // initialize variables
  Room currentRoom;
  Point tempPoint;
  Exit e1, e2;
  std::vector<Room> rooms;
  std::vector<Point> c1, c2;
  int r1 ,r2, pr1, pr2;
  json jsonObject_;

  // building the data
  r1 = 0;
  r2 = 1;
  pr1 = -1;
  pr2 = 0;

  tempPoint.x = 1;
  tempPoint.y = 2;
  c1.push_back(tempPoint);
  tempPoint.x = 3;
  tempPoint.y = 4;
  c1.push_back(tempPoint);
  tempPoint.x = 5;
  tempPoint.y = 6;
  c1.push_back(tempPoint);
  tempPoint.x = 7;
  tempPoint.y = 8;
  c1.push_back(tempPoint);

  tempPoint.x = 100;
  tempPoint.y = 200;
  e1.exitPoint1 = tempPoint;
  tempPoint.x = 300;
  tempPoint.y = 400;
  e1.exitPoint2 = tempPoint;


  tempPoint.x = 9;
  tempPoint.y = 10;
  c2.push_back(tempPoint);
  tempPoint.x = 10;
  tempPoint.y = 11;
  c2.push_back(tempPoint);
  tempPoint.x = 12;
  tempPoint.y = 13;
  c2.push_back(tempPoint);
  tempPoint.x = 14;
  tempPoint.y = 15;
  c2.push_back(tempPoint);

  tempPoint.x = 500;
  tempPoint.y = 600;
  e2.exitPoint1 = tempPoint;
  tempPoint.x = 700;
  tempPoint.y = 800;
  e2.exitPoint2 = tempPoint;

  currentRoom.roomID = r1;
  currentRoom.corners = c1;
  currentRoom.previousRoom = pr1;
  currentRoom.exit = e1;
  rooms.push_back(currentRoom);

  currentRoom.roomID = r2;
  currentRoom.corners = c2;
  currentRoom.previousRoom = pr2;
  currentRoom.exit = e2;
  rooms.push_back(currentRoom);

  // copy the data into a json object
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

  // json o = j[0];
  //
  // for (json::iterator it = o.begin(); it != o.end(); ++it) {
  //   std::cout << it.key() << " : " << it.value() << "\n";
  // }
  // serialize the json to a file
  std::ofstream output("rooms.json");
  output << std::setw(4) << jsonObject_ << std::endl;
  // std::cout << o.find("x") << '\n';
  std::cout << "Done!" << std::endl;
}
