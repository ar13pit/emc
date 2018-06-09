#include <iostream>
#include "worldmodel.h"

using namespace std;


int main() {
    /* code */
    Location cl;
    WorldModel wm;

    cl = wm.get_currentLocation();
    std::cout << "currentLocation: " << cl << '\n';

    wm.set_currentLocation(IN_ROOM);

    cl = wm.get_currentLocation();
    std::cout << "currentLocation: " << cl << '\n';


    return 0;
}
