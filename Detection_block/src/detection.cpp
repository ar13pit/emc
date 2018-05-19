#include "detection.h"
#include "config.h"

    bool Detection::getSensorData() {
        if(inOut->readLaserData(laser)) {
            return true;
        } else {
            return false;
        }
    }

    void Detection::filterLRFData(emc::LaserData* laser, int nFilterPoints){
        for(int i = 0; i<nFilterPoints; ++i){
           laser->ranges[i] = laser->ranges[nFilterPoints];
           laser->ranges[laser->ranges.size()-1-i] = laser->ranges[laser->ranges.size()-1-nFilterPoints];
        }

        /*for(int i = 16; i < laser->ranges.size()-15; ++i) {
               if(laser->ranges[i] < aux && laser->ranges[i] > LRF_THRESH) {
                   aux = laser->ranges[i];
               }
           }
         minDistance_ = aux;*/

    }

    /*bool Detection::wallDetected(double minDistance) {
        if(minDistance < MIN_DIST_TO_WALL) {
            return true;
        } else {
            return false;
        }
    }*/
