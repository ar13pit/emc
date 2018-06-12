#ifndef planning_H
#define planning_H

#include "stateMachine.h"
#include "detection.h"
#include "worldModel.h"
#include "config.h"
#include "detection.h"
#include "iostream"
#include "math.h"
#include "helper.h"

#include <cmath>
#include <vector>

    class WorldModel;

//bool state_machine(struct High_state high_st,struct Low_state low_st);

class Planning{
private:

    Destination dest;

public:


    Destination driveToPoint(Point navigateTo, WorldModel *worldModel);
    Destination setpointInCorridor();               //Navigate Pico through Corridor
    Destination getAwayFromWall(Low_State lowSt, WorldModel *worldModel);
    Destination driveInRoom(WorldModel *worldModel);
    Destination parkPico(WorldModel *worldModel);

    Destination get_Destination();

    Point getNearbyExitPoint(Room closestRoom, WorldModel *worldModel);
    Point getStartPos();
    Point getThroughtExitPoint(Room roomFromMapping, WorldModel *worldModel);


    Planning(){
        return;
    }

};
#endif //planning_H
