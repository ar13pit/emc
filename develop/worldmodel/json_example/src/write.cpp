#include <vector>
#include <iostream>
#include <fstream>
#include "./json.hpp"
#include "./corner.h"

using json = nlohmann::json;

int main()
{
  // building the data
  std::vector<Corner> corners;
  corners.push_back(Corner(3, 4));
  corners.push_back(Corner(5, 6));

  // copy the data into a json object
  json j;
  for (auto corner : corners)
  {
    json c = { { "x", corner.x_ }, { "y", corner.y_ } };
    j.push_back(c);
  }

  json o = j[0];

  for (json::iterator it = o.begin(); it != o.end(); ++it) {
    std::cout << it.key() << " : " << it.value() << "\n";
  }
  // serialize the json to a file
  std::ofstream output("corners.json");
  output << std::setw(4) << j << std::endl;
  // std::cout << o.find("x") << '\n';
  std::cout << "Done!" << std::endl;
}
