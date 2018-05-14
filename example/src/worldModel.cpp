#include "worldModel.h"

double* WorldModel::getMinimumDistance()
{
    return &minDistance_;
}

void WorldModel::setMinimumDistance(emc::LaserData* laser)
{
     double aux = laser->ranges[0];
     for(int i = 15; i < laser->ranges.size()-15; ++i) {
            if(laser->ranges[i] < aux && laser->ranges[i] > LRF_THRESH) {
                aux = laser->ranges[i];
            }
        }
      minDistance_ = aux;
}
