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

    void Detection::saveLRFScan(emc::LaserData* laser){
        int nFilterPoints = 15;
        double a = (15*laser->angle_increment) + laser->angle_min;
        for(unsigned int i = nFilterPoints; i < laser->ranges.size()-nFilterPoints; ++i)
        {
            if(laser->ranges[i]>0.001){
                Detection::LatestLaserScan[i-nFilterPoints].dist = laser->ranges[i];
                Detection::LatestLaserScan[i-nFilterPoints].angle = -a;
                Detection::LatestLaserScan[i-nFilterPoints].x = sin(-a) * laser->ranges[i];
                Detection::LatestLaserScan[i-nFilterPoints].y = cos(-a) * laser->ranges[i];
            }
            else{ // 30 meters if range is below threshold
                Detection::LatestLaserScan[i-nFilterPoints].dist = 30;
                Detection::LatestLaserScan[i-nFilterPoints].angle = -a;
                Detection::LatestLaserScan[i-nFilterPoints].x = sin(-a) * 30;
                Detection::LatestLaserScan[i-nFilterPoints].y = cos(-a) * 30;
            }

            a += laser->angle_increment;
        }

    }

    CorridorWalls Detection::findCorridorWalls(){
        bool detectedRight = false;
        bool detectedLeft = false;
        Point right1;
        Point right2;
        Point left1;
        Point left2;
        double aFit;
        double bFit;
        int nPointsSearch = 80;

        //Look for right wall
        for(unsigned int i = 50; i < 250; i = i + 20){
            if(LatestLaserScan[i].dist < 1.5){
                if(Detection::lineFit(aFit, bFit, i, i+nPointsSearch) ){
                    right1.y = -1;
                    right2.y = 1;
                    right1.x = (right1.y - bFit)/aFit;
                    right2.x = (right2.y - bFit)/aFit;
                    detectedRight = true;
                    std::cout << "Detected right wall" << std::endl;
                    i = 251;
                }
            }
        }

        for(unsigned int i = 940; i > 700; i = i - 20){
            if(LatestLaserScan[i].dist < 1.5){
                if(Detection::lineFit(aFit, bFit, i-nPointsSearch, i) ){
                    left1.y = -1;
                    left2.y = 1;
                    left1.x = (left1.y - bFit)/aFit;
                    left2.x = (left2.y - bFit)/aFit;
                    detectedLeft = true;
                    std::cout << "Detected left wall" << std::endl;
                    i = 699;
                }
            }
        }

        CorridorWalls walls;
        walls.rightWall1 = right1;
        walls.rightWall2 = right2;
        walls.leftWall1 = left1;
        walls.leftWall2 = left2;
        walls.escaped = !(detectedRight && detectedLeft);
        return walls;

    }

    Exit Detection::findExit(){


    }

    //Fit a line y = a*x + b between a set of specified points (LatestLaserScan points between index firstPoint and lastPoint)
    bool Detection::lineFit(double &aFit, double &bFit, int firstPoint, int lastPoint){
        unsigned int nFitPoints = 5; //Amount of points that are used to fit line


        int nAveragePoints = (lastPoint - firstPoint)/nFitPoints;
        Point lineFitPoint[nFitPoints];

        for(unsigned int i = 0; i < nFitPoints; ++i){
            double sumx = 0;
            double sumy = 0;
            for(unsigned int j = 0; j < nAveragePoints; ++j){
                sumx = sumx + LatestLaserScan[firstPoint + j + i*nAveragePoints].x;
                sumy = sumy + LatestLaserScan[firstPoint + j + i*nAveragePoints].y;
            }
            lineFitPoint[i].x = sumx/nAveragePoints;
            lineFitPoint[i].y = sumy/nAveragePoints;

            std::cout << "Point " << i << ": (" << lineFitPoint[i].x << ", " << lineFitPoint[i].y << ")" << std::endl;
        }


        //Fit line y = a*x + b through the first and last (averaged) point

        aFit = (lineFitPoint[nFitPoints-1].y - lineFitPoint[0].y)/(lineFitPoint[nFitPoints-1].x - lineFitPoint[0].x);
        bFit = lineFitPoint[0].y - aFit*lineFitPoint[0].x;

        double fitError = 0;

        if(aFit < -1 || aFit > 1){ // Evaluate x-coordinates for given y
            for(unsigned int i = 1; i < nFitPoints - 1; ++i){
                double xEval = (lineFitPoint[i].y - bFit)/aFit;
                double errorPoint = xEval - lineFitPoint[i].x;
                fitError = fitError + pow(errorPoint,2);
            }
        } else{ // Evaluate y-coordinates for given x
            for(unsigned int i = 1; i < nFitPoints - 1; ++i){
                double yEval = aFit * lineFitPoint[i].x + bFit;
                double errorPoint = yEval - lineFitPoint[i].y;
                fitError = fitError + pow(errorPoint,2);
            }
        }
        double RMSerror = sqrt(fitError);

        //Total threshold for linefit (larger when more points are evaluated or when points are further apart from each other)
        double errorThresh = LINEFITTHRESH*(lastPoint-firstPoint)*(nFitPoints - 2);

        if (RMSerror < errorThresh){
            return true;
        } else{
            return false;
        }

    }

    Point Detection::findFurthestPoint(){
        int imax = 0;
        double max = 0;
        for(int i = 0; i < 1000-2*15; ++i){
            if (LatestLaserScan[i].dist > max){
                max = LatestLaserScan[i].dist;
                imax = i;
            }
        }
        return LatestLaserScan[imax];
    }



    /*bool Detection::wallDetected(double minDistance) {
        if(minDistance < MIN_DIST_TO_WALL) {
            return true;
        } else {
            return false;
        }
    }*/
