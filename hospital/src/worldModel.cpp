#include "worldModel.h"

void WorldModel::set_destination(Destination dest){
    destination_ = dest;
}


/*
double* WorldModel::getMinimumDistance()
{
    return &minDistance_;
}

void WorldModel::setMinimumDistance(emc::LaserData* laser)
{
     double aux = laser->ranges[15];
     for(int i = 16; i < laser->ranges.size()-15; ++i) {
            if(laser->ranges[i] < aux && laser->ranges[i] > LRF_THRESH) {
                aux = laser->ranges[i];
            }
        }
      minDistance_ = aux;
}
*/
