#include "detection.h"


/*-------------------------------------------------------------------------------
 *                  Suplementary functions
 *-------------------------------------------------------------------------------
 * */


bool Detection::getSensorData() {
    if(inOut->readLaserData(laser_)) {
        return true;
    } else {
        return false;
    }
}







void Detection::filterLRFData(int nFilterPoint_dets) {
    for(int i = 0; i<nFilterPoint_dets; ++i){
        laser_.ranges[i] = laser_.ranges[nFilterPoint_dets];
        laser_.ranges[laser_.ranges.size()-1-i] = laser_.ranges[laser_.ranges.size()-1-nFilterPoint_dets];
    }

}







void Detection::saveLRFScan() {
    int nFilterPoint_dets = 15;
    double a = (15*laser_.angle_increment) + laser_.angle_min;

    for(unsigned int i = nFilterPoint_dets; i < laser_.ranges.size()-nFilterPoint_dets; ++i)
    {
        if(laser_.ranges[i]>0.001){
            LatestLaserScan[i-nFilterPoint_dets].dist = laser_.ranges[i];
            LatestLaserScan[i-nFilterPoint_dets].angle = -a;
            LatestLaserScan[i-nFilterPoint_dets].x = sin(-a) * laser_.ranges[i];
            LatestLaserScan[i-nFilterPoint_dets].y = cos(-a) * laser_.ranges[i];
        }
        else{ // 30 meters if range is below threshold
            LatestLaserScan[i-nFilterPoint_dets].dist = 30;
            LatestLaserScan[i-nFilterPoint_dets].angle = -a;
            LatestLaserScan[i-nFilterPoint_dets].x = sin(-a) * 30;
            LatestLaserScan[i-nFilterPoint_dets].y = cos(-a) * 30;
        }

        a += laser_.angle_increment;
    }

}






//Find 2 lines of the walls in the corridor
void Detection::findCorridorWalls() {
    bool detectedRight = false;
    bool detectedLeft = false;
    Point_det right1;
    Point_det right2;
    Point_det left1;
    Point_det left2;
    double aFit;
    double bFit;
    int nPoint_detsSearch = 80;

    //Look for left wall
    for(unsigned int i = 700; i < 940; i = i + 20){
        if(LatestLaserScan[i].dist < 1.5){
            if(Detection::lineFit(aFit, bFit, i-nPoint_detsSearch, i) ){
                left1.y = -1;
                left2.y = 1;
                left1.x = (left1.y - bFit)/aFit;
                left2.x = (left2.y - bFit)/aFit;
                detectedLeft = true;
                std::cout << "Detected left wall" << std::endl;
                i = 941;
            }
        }
    }


    //Look for right wall
    for(unsigned int i = 300; i > 50; i = i - 20){
        if(LatestLaserScan[i].dist < 1.5){
            if(Detection::lineFit(aFit, bFit, i, i+nPoint_detsSearch) ){
                right1.y = -1;
                right2.y = 1;
                right1.x = (right1.y - bFit)/aFit;
                right2.x = (right2.y - bFit)/aFit;
                detectedRight = true;
                std::cout << "Detected right wall" << std::endl;
                i = 21;
            }
        }
    }


//    CorridorWalls walls;
    data_.corridor.leftWall1 = left1;
    data_.corridor.leftWall2 = left2;
    data_.corridor.rightWall1 = right1;
    data_.corridor.rightWall2 = right2;
    data_.corridor.escaped = !(detectedRight && detectedLeft);
/*    walls.rightWall1 = right1;
    walls.rightWall2 = right2;
    walls.leftWall1 = left1;
    walls.leftWall2 = left2;
    walls.escaped = !(detectedRight && detectedLeft);
    return walls;*/

}







void Detection::findExit() {

    double detectLargerThresh = 1.05;
    double detectSmallerThresh = 0.95;
    int nPoint_detsThresh = 4;                                                      // Amount of Point_dets larger of smaller than expected before action is being undertaken
    int nPoint_detsSearch = 80;

    int iExit1 = 0;
    int iExit2 = 0;

    double aFit; double bFit;

    for (unsigned int i = 0; i < (1000 - 2*15 - nPoint_detsSearch); i = i+10){
        if(lineFit(aFit, bFit, i, i+nPoint_detsSearch)){                            // Line found, next: search for deviation in positive direction
            int nLarger = 0;                                                        // Amount of Point_dets larger than a certain treshold, compared with linefit data
            int nSmaller = 0;                                                       // Amount of Point_dets smaller than a certain treshold, compared with linefit data
            for (int j = i+nPoint_detsSearch; j < 1000 - 2*15 - nPoint_detsSearch; ++j){
                double xLine, yLine, distLine;
                if(aFit < -1 || aFit > 1){                                          // Evaluate x-coordinates for given y
                    yLine = LatestLaserScan[j].y;
                    xLine = (yLine - bFit)/aFit;
                }
                else{                                                               // Evaluate y-coordinates for given x
                    xLine = LatestLaserScan[j].x;
                    yLine = aFit*xLine + bFit;

                }
                distLine = sqrt(pow(xLine,2) + pow(yLine,2));

                if (distLine * detectLargerThresh < LatestLaserScan[j].dist){
                    nLarger = nLarger + 1;
                    nSmaller = 0;
                }
                else if (distLine * detectSmallerThresh > LatestLaserScan[j].dist){
                    nLarger = 0;
                    nSmaller = nSmaller + 1;
                }
                else{
                    nLarger = 0; nSmaller = 0;
                }

                if(nSmaller > nPoint_detsThresh){                                   // Corner detected
                    i = j - 10;
                    j = 1000;                                                       // Break out
                }
                if (nLarger > nPoint_detsThresh){                                   // Exit detected
                    std::cout << "ExitPoint_det 1 detected!";
                    iExit1 = j - 7;

                    // Start searching for the second Point_det of the exit
                    int nEqual = 0;

                    for (int k = j; k < 1000 - 2*15 - nPoint_detsSearch; ++k){

                        double xLine; double yLine; double distLine;
                        if(aFit < -1 || aFit > 1){                                  // Evaluate x-coordinates for given y
                            yLine = LatestLaserScan[k].y;
                            xLine = (yLine - bFit)/aFit;
                        }
                        else{                                                       // Evaluate y-coordinates for given x
                            xLine = LatestLaserScan[k].x;
                            yLine = aFit*xLine + bFit;

                        }
                        distLine = sqrt(pow(xLine,2) + pow(yLine,2));

                        if (distLine * detectLargerThresh > LatestLaserScan[k].dist && distLine * detectSmallerThresh < LatestLaserScan[k].dist){
                            nEqual = nEqual + 1;
                        }
                        else{
                            nEqual = 0;
                        }

                        if (nEqual > 5){
                            iExit2 = k - 5;
                            data_.exit.exitPoint_det1 = LatestLaserScan[iExit1];
                            data_.exit.exitPoint_det2 = LatestLaserScan[iExit2];
                            data_.exit.detected = true;
                        }
                    }


                }

            }
        }
    }
    data_.exit.exitPoint_det1 = LatestLaserScan[0];
    data_.exit.exitPoint_det2 = LatestLaserScan[0];
    data_.exit.detected = false;
    //return exit;

}






//Fit a line y = a*x + b between a set of specified Point_dets (LatestLaserScan Point_dets between index firstPoint_det and lastPoint_det)
bool Detection::lineFit(double &aFit, double &bFit, int firstPoint_det, int lastPoint_det){
    unsigned int nFitPoint_dets = 5; //Amount of Point_dets that are used to fit line


    int nAveragePoint_dets = (lastPoint_det - firstPoint_det)/nFitPoint_dets;
    Point_det lineFitPoint_det[nFitPoint_dets];

    for(unsigned int i = 0; i < nFitPoint_dets; ++i){
        double sumx = 0;
        double sumy = 0;
        for(unsigned int j = 0; j < nAveragePoint_dets; ++j){
            sumx = sumx + LatestLaserScan[firstPoint_det + j + i*nAveragePoint_dets].x;
            sumy = sumy + LatestLaserScan[firstPoint_det + j + i*nAveragePoint_dets].y;
        }
        lineFitPoint_det[i].x = sumx/nAveragePoint_dets;
        lineFitPoint_det[i].y = sumy/nAveragePoint_dets;

        //std::cout << "Point_det " << i << ": (" << lineFitPoint_det[i].x << ", " << lineFitPoint_det[i].y << ")" << std::endl;
    }


    //Fit line y = a*x + b through the first and last (averaged) Point_det

    aFit = (lineFitPoint_det[nFitPoint_dets-1].y - lineFitPoint_det[0].y)/(lineFitPoint_det[nFitPoint_dets-1].x - lineFitPoint_det[0].x);
    bFit = lineFitPoint_det[0].y - aFit*lineFitPoint_det[0].x;

    double fitError = 0;

    if(aFit < -1 || aFit > 1){ // Evaluate x-coordinates for given y
        for(unsigned int i = 1; i < nFitPoint_dets - 1; ++i){
            double xEval = (lineFitPoint_det[i].y - bFit)/aFit;
            double errorPoint_det = xEval - lineFitPoint_det[i].x;
            fitError = fitError + pow(errorPoint_det,2);
        }
    } else{ // Evaluate y-coordinates for given x
        for(unsigned int i = 1; i < nFitPoint_dets - 1; ++i){
            double yEval = aFit * lineFitPoint_det[i].x + bFit;
            double errorPoint_det = yEval - lineFitPoint_det[i].y;
            fitError = fitError + pow(errorPoint_det,2);
        }
    }
    double RMSerror = sqrt(fitError);

    //Total threshold for linefit (larger when more Point_dets are evaluated or when Point_dets are further apart from each other)
    double errorThresh = LINEFITTHRESH*(lastPoint_det-firstPoint_det)*(nFitPoint_dets - 2);

    if (RMSerror < errorThresh){
        return true;
    } else{
        return false;
    }

}




void Detection::findFurthestPoint_det() {
    int imax = 0;
    double max = 0;
    for(int i = 0; i < 1000-2*15; ++i){
        if (LatestLaserScan[i].dist > max){
            max = LatestLaserScan[i].dist;
            imax = i;
        }
    }
    data_.furthest_point = LatestLaserScan[imax];
}




Detection_data Detection::get_Detection_data() {
    return data_;
};




/*bool Detection::wallDetected(double minDistance) {
        if(minDistance < MIN_DIST_TO_WALL) {
            return true;
        } else {
            return false;
        }
    }*/
