#include <fstream>
#include <cassert>
#include <iostream>
#include "./json.hpp"
#include "./corner.h"

using json = nlohmann::json;

int main()
{
  // read the json object from a file
  std::ifstream i("corners.json");
  json j;
  i >> j;

  std::vector<Corner> corners;

  // copy the data from a json object
  assert(j.is_array());
  for (const auto& c : j)
  {
    assert(c.is_object());
    std::cout << c.is_object() << '\n';
    assert(c.find("x") != c.end());  // check for key x
    assert(c.find("y") != c.end());  // check for key y

    corners.push_back(Corner(c["x"], c["y"]));
  }

  // now we can use our corners array again
  for (const auto& corner : corners)
  {
    std::cout << "Corner x=" << corner.x_ << ", y=" << corner.y_ << std::endl;
  }

  std::cout << "Done!" << std::endl;
}
