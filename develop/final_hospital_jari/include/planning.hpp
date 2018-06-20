#include <iostream>
#include <cmath>
#include <vector>

#include "worldModel.hpp"
#include "config.hpp"
#include "helper.hpp"

#ifndef planning_H
#define planning_H



//bool state_machine(struct High_state high_st,struct Low_state low_st);

class Planning {

    Point dest;
    WorldModel* WM;

public:

    Planning (WorldModel* worldmodel);

    Point driveToPoint (Point navigateTo, Point curPos);
    Point setpointInCorridor ();               //Navigate Pico through Corridor
    Point getAwayFromWall ();
    Point driveInRoom ();
    Point parkPico ();

    Point get_Destination ();

    Point getNearbyExitPoint (Room closestRoom);
    Point getStartPos ();
    Point getThroughtExitPoint (Room roomFromMapping);



};

#endif //planning_H
