#include <vector>
#include <iostream>
#include <fstream>
#include "./json.hpp"
#include "./corner.h"
#include <string>

using json = nlohmann::json;

int main()
{
  // building the data
  std::vector<Corner> corners;
  corners.push_back(Corner(3, 4));
  corners.push_back(Corner(5, 6));
  corners.push_back(Corner(7, 8));
  corners.push_back(Corner(9, 10));

  // copy the data into a json object
  json cornerJSON, j2, j;
  int cornerID = 0;
  for (auto corner : corners)
  {
    json c = { { "x", corner.x_ }, { "y", corner.y_ } } ;
    std::cout << c << '\n';
    cornerJSON.emplace(std::to_string(cornerID), c);
    cornerID++;
  }

  j2 = j1;

  j.push_back(j1);
  j.push_back(j2);

  std::cout << j << '\n';
  std::cout << j[0] << '\n';
  std::cout << j[1] << '\n';

  // serialize the json to a file
  std::ofstream output("corners.json");
  output << std::setw(4) << j << std::endl;

  std::cout << "Done!" << std::endl;
}
