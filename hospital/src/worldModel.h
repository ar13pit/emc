#include "config.h"
#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>

#include "detection.h"
#include "planning.h"
#include "main2.hpp"

#ifndef worldModel_H
#define worldModel_H

typedef struct {
    std::vector<Point_det> corners;
    std::vector<Exit> exits;
} Room;


// this is based on an assumption that corridor has most of the rooms connected
// we first explore the corridor (assume it is straight, any T-junctions are rooms)
typedef struct {
    std::vector<Exit> exits;
} CCorridor;    //name has to be changed (conflicting class in planning for escape room challenge)

/*
class WorldModel
        {
private:
    double minDistance_;

public:
WorldModel(){
        minDistance_ = MAX_RANGE_LRF;
    }

    double* getMinimumDistance();
    void setMinimumDistance(emc::LaserData* laser); // Method to determine the minimum distance to a wall
};
*/
#endif //worldModel_H
