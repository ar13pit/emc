#include "detection.h"
#include "config.h"


    void Detection::average_CornersAndExits(){
         emc::Rate rate(EXECUTION_RATE);


        // AVERAGING MAGIC NUMBERS
        int nAverages = 10;
        double distance_thresh = 0.01;
        int nLarger = 5;

        // RIGHT-LEFT AVERAGING INITILISING
        Corner AverageCornerPoint_RL[100];
        Corner AverageCornerPoint_RL_final[20];
        Exit AverageExit_RL[100];
        Exit AverageExit_RL_final[20];
        int nAverageCornerPoint_RL[100];
        int nAverageExit_RL[100];

        for (int i = 0; i < 100; ++i){
            nAverageCornerPoint_RL[i] = 0;
            AverageCornerPoint_RL[i].detected = false;
            nAverageExit_RL[i] = 0;
            AverageExit_RL[i].detected = false;
        }

        // LEFT-RIGHT AVERAGING INITILISING
        Corner_LR AverageCornerPoint_LR[100];
        Corner_LR AverageCornerPoint_LR_final[20];
        Exit_LR AverageExit_LR[100];
        Exit_LR AverageExit_LR_final[20];
        int nAverageCornerPoint_LR[100];
        int nAverageExit_LR[100];

        for (int i = 0; i < 100; ++i){
            nAverageCornerPoint_LR[i] = 0;
            AverageCornerPoint_LR[i].detected = false;
            nAverageExit_LR[i] = 0;
            AverageExit_LR[i].detected = false;
        }


        for (int i = 0; i < nAverages; ++i){

            rate.sleep();
            if(inOut->ok()){
            if(getSensorData()){
                saveLRFScan();
            }
            }

            rate.sleep();


        // RIGHT-LEFT AVG. EXITS AND CORNERS
        findExitsAndCorners_RL();

            for(int j = 0; j < 20; j = j+1){ //20 is length of Corners_RL, be aware of robustness issues when changing this length

                if(Corners_RL[j].detected){
                    bool corner_detected = false;
                    for(int k = 0; k < 100; k = k+1){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
                        if(AverageCornerPoint_RL[k].detected){
                           double distance = sqrt( pow(AverageCornerPoint_RL[k].cornerPoint.x - Corners_RL[j].cornerPoint.x,2) +  pow(AverageCornerPoint_RL[k].cornerPoint.y - Corners_RL[j].cornerPoint.y,2));


                            if(distance < distance_thresh){
                                ++nAverageCornerPoint_RL[k];
                                AverageCornerPoint_RL[k].cornerPoint.x = (AverageCornerPoint_RL[k].cornerPoint.x * (nAverageCornerPoint_RL[k] - 1) + Corners_RL[j].cornerPoint.x)/nAverageCornerPoint_RL[k]; //Weighted average over x of cornerPoints
                                AverageCornerPoint_RL[k].cornerPoint.y = (AverageCornerPoint_RL[k].cornerPoint.y * (nAverageCornerPoint_RL[k] - 1) + Corners_RL[j].cornerPoint.y)/nAverageCornerPoint_RL[k]; //Weighted average over y of cornerPoints
                                corner_detected = true;
                                break;
                            }
                    }
                        else{
                             break;
                        }
                    }

                    if(corner_detected==false){
                        for(int k = 0; k < 100; k = k+1){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
                           if(AverageCornerPoint_RL[k].detected == false){
                               AverageCornerPoint_RL[k].detected = true;
                               AverageCornerPoint_RL[k] = Corners_RL[j];
                               nAverageCornerPoint_RL[k] = 1;
                               break;
                           }
                        }
                    }
                }
            }

            for(int j = 0; j < 20; j = j+1){ //20 is length of Corners_RL, be aware of robustness issues when changing this length

                if(Exits_RL[j].detected){
                    bool exit_detected = false;
                    for(int k = 0; k < 100; k = k+1){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
                        if(AverageExit_RL[k].detected){
                           double centerExitx = 0.5 * Exits_RL[j].exitPoint1.x + 0.5 * Exits_RL[j].exitPoint2.x;
                           double centerExity = 0.5 * Exits_RL[j].exitPoint1.y + 0.5 * Exits_RL[j].exitPoint2.y;
                           double average_centerExitx = 0.5 * AverageExit_RL[j].exitPoint1.x + 0.5 * AverageExit_RL[j].exitPoint2.x;
                           double average_centerExity = 0.5 * AverageExit_RL[j].exitPoint1.y + 0.5 * AverageExit_RL[j].exitPoint2.y;
                           double distance = sqrt( pow(centerExitx - average_centerExitx,2) +  pow(centerExity - average_centerExity,2));

                            if(distance < distance_thresh){
                                ++nAverageExit_RL[k];
                                AverageExit_RL[k].exitPoint1.x = (AverageExit_RL[k].exitPoint1.x * (nAverageExit_RL[k] - 1) + Exits_RL[j].exitPoint1.x)/nAverageExit_RL[k]; //Weighted average over x of cornerPoints
                                AverageExit_RL[k].exitPoint2.x = (AverageExit_RL[k].exitPoint2.x * (nAverageExit_RL[k] - 1) + Exits_RL[j].exitPoint2.x)/nAverageExit_RL[k]; //Weighted average over x of cornerPoints

                                AverageExit_RL[k].exitPoint1.y = (AverageExit_RL[k].exitPoint1.y * (nAverageExit_RL[k] - 1) + Exits_RL[j].exitPoint1.y)/nAverageExit_RL[k]; //Weighted average over y of cornerPoints
                                AverageExit_RL[k].exitPoint2.y = (AverageExit_RL[k].exitPoint2.y * (nAverageExit_RL[k] - 1) + Exits_RL[j].exitPoint2.y)/nAverageExit_RL[k]; //Weighted average over y of cornerPoints
                                std::cout << "x1_RL = " << AverageExit_RL_final[k].exitPoint1.x << " y_RL = " << AverageExit_RL_final[k].exitPoint1.y << std::endl;
                                std::cout << "x2_RL = " << AverageExit_RL_final[k].exitPoint2.x << " y2_RL = " << AverageExit_RL_final[k].exitPoint2.y << std::endl;

                                exit_detected = true;
                                break;

                            }
                        }
                        else{
                            break;
                        }
                    }
                    if(exit_detected==false){
                        for(int k = 0; k < 100; k = k+1){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
                           if(AverageExit_RL[k].detected == false){
                               AverageExit_RL[k].detected = true;
                               AverageExit_RL[k] = Exits_RL[j];
                               nAverageExit_RL[k] = 1;
                               break;
                           }
                        }
                    }

                }

            }


            // LEFT-RIGHT AVG. EXITS AND CORNERS
            findExitsAndCorners_LR();


            for(int j = 0; j < 20; j = j+1){ //20 is length of Corners_RL, be aware of robustness issues when changing this length

                if(Corners_LR[j].detected){
                    bool corner_detected = false;
                    for(int k = 0; k < 100; k = k+1){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
                        if(AverageCornerPoint_LR[k].detected){
                           double distance = sqrt( pow(AverageCornerPoint_LR[k].cornerPoint_LR.x - Corners_LR[j].cornerPoint_LR.x,2) +  pow(AverageCornerPoint_LR[k].cornerPoint_LR.y - Corners_LR[j].cornerPoint_LR.y,2));

                            if(distance < distance_thresh){
                                ++nAverageCornerPoint_LR[k];
                                AverageCornerPoint_LR[k].cornerPoint_LR.x = (AverageCornerPoint_LR[k].cornerPoint_LR.x * (nAverageCornerPoint_LR[k] - 1) + Corners_LR[j].cornerPoint_LR.x)/nAverageCornerPoint_LR[k]; //Weighted average over x of cornerPoints
                                AverageCornerPoint_LR[k].cornerPoint_LR.y = (AverageCornerPoint_LR[k].cornerPoint_LR.y * (nAverageCornerPoint_LR[k] - 1) + Corners_LR[j].cornerPoint_LR.y)/nAverageCornerPoint_LR[k]; //Weighted average over y of cornerPoints

                                corner_detected = true;
                                break;

                            }
                        }
                        else{
                            break;
                        }
                    }
                    if(corner_detected==false){
                        for(int k = 0; k < 100; k = k+1){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
                           if(AverageCornerPoint_LR[k].detected == false){
                               AverageCornerPoint_LR[k].detected = true;
                               AverageCornerPoint_LR[k] = Corners_LR[j];
                               nAverageCornerPoint_LR[k] = 1;
                               break;
                           }
                        }
                    }
                }
            }

            for(int j = 0; j < 20; j = j+1){ //20 is length of Corners_RL, be aware of robustness issues when changing this length

                if(Exits_LR[j].detected){
                    bool exit_detected = false;
                    for(int k = 0; k < 100; k = k+1){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
                        if(AverageExit_LR[k].detected){
                           double centerExitx = 0.5 * Exits_LR[j].exitPoint1_LR.x + 0.5 * Exits_LR[j].exitPoint2_LR.x;
                           double centerExity = 0.5 * Exits_LR[j].exitPoint1_LR.y + 0.5 * Exits_LR[j].exitPoint2_LR.y;
                           double average_centerExitx = 0.5 * AverageExit_LR[j].exitPoint1_LR.x + 0.5 * AverageExit_LR[j].exitPoint2_LR.x;
                           double average_centerExity = 0.5 * AverageExit_LR[j].exitPoint1_LR.y + 0.5 * AverageExit_LR[j].exitPoint2_LR.y;
                           double distance = sqrt( pow(centerExitx - average_centerExitx,2) +  pow(centerExity - average_centerExity,2));

                            if(distance < distance_thresh){
                                ++nAverageExit_LR[k];
                                AverageExit_LR[k].exitPoint1_LR.x = (AverageExit_LR[k].exitPoint1_LR.x * (nAverageExit_LR[k] - 1) + Exits_LR[j].exitPoint1_LR.x)/nAverageExit_LR[k]; //Weighted average over x of cornerPoints
                                AverageExit_LR[k].exitPoint2_LR.x = (AverageExit_LR[k].exitPoint2_LR.x * (nAverageExit_LR[k] - 1) + Exits_LR[j].exitPoint2_LR.x)/nAverageExit_LR[k]; //Weighted average over x of cornerPoints

                                AverageExit_LR[k].exitPoint1_LR.y = (AverageExit_LR[k].exitPoint1_LR.y * (nAverageExit_LR[k] - 1) + Exits_LR[j].exitPoint1_LR.y)/nAverageExit_LR[k]; //Weighted average over y of cornerPoints
                                AverageExit_LR[k].exitPoint2_LR.y = (AverageExit_LR[k].exitPoint2_LR.y * (nAverageExit_LR[k] - 1) + Exits_LR[j].exitPoint2_LR.y)/nAverageExit_LR[k]; //Weighted average over y of cornerPoints

                                exit_detected = true;
                                break;

                            }
                        }
                        else{
                            break;
                        }
                    }
                    if(exit_detected==false){
                        for(int k = 0; k < 100; k = k+1){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
                           if(AverageExit_LR[k].detected == false){
                               AverageExit_LR[k].detected = true;
                               AverageExit_LR[k] = Exits_LR[j];
                               nAverageExit_LR[k] = 1;
                               break;
                           }
                        }
                    }

                }

            }

        }


        for(int i = 0; i < 20; ++i){
                Corners_RL[i].detected = false;
                Exits_RL[i].detected = false;//
        }


        int j = 0;
        for(int i = 0; i < 100; ++i){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
            if(nAverageCornerPoint_RL[i] > nLarger){

                //std::cout << nAverageCornerPoint_RL[i] << std::endl;

                Corners_RL[j] = AverageCornerPoint_RL[i];
                AverageCornerPoint_RL_final[j] = AverageCornerPoint_RL[i];
                //std::cout << nAverageCornerPoint_RL[i] << std::endl;
                ++j;
            }
        }


        j = 0;
        for(int i = 0; i < 100; ++i){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
            if(nAverageExit_RL[i] > nLarger){


                Exits_RL[j] = AverageExit_RL[i];
                AverageExit_RL_final[j] = AverageExit_RL[i];
                //std::cout << nAverageExit_RL[i] << std::endl;
                ++j;
            }
        }


        for(int i = 0; i < 20; ++i){
                Corners_LR[i].detected = false;
                Exits_LR[i].detected = false;
        }

        j = 0;
        for(int i = 0; i < 100; ++i){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
            if(nAverageCornerPoint_LR[i] > nLarger){

                Corners_LR[j] = AverageCornerPoint_LR[i];
                AverageCornerPoint_LR_final[j] = AverageCornerPoint_LR[i];
                ++j;
            }
        }

        j = 0;
        for(int i = 0; i < 100; ++i){ //100 is length of AverageCornerPoint, be aware of robustness issues when changing this length
            if(nAverageExit_RL[i] > nLarger){

                Exits_LR[j] = AverageExit_LR[i];
                AverageExit_LR_final[j] = AverageExit_LR[i];
                ++j;
            }
        }
    }

    bool Detection::getSensorData() {
        if(inOut->readLaserData(laser)) {
            return true;
        } else {
            return false;
        }
    }

    void Detection::filterLRFData(int nFilterPoints){
        for(int i = 0; i<nFilterPoints; ++i){
           laser.ranges[i] = laser.ranges[nFilterPoints];
           laser.ranges[laser.ranges.size()-1-i] = laser.ranges[laser.ranges.size()-1-nFilterPoints];
        }
    }

    void Detection::saveLRFScan(){
        unsigned int nFilterPoints = 15;
        double a = (15*laser.angle_increment) + laser.angle_min;
        for(unsigned int i = nFilterPoints; i < laser.ranges.size()-nFilterPoints; ++i)
        {
            if(laser.ranges[i]>0.001){
                Detection::LatestLaserScan[i-nFilterPoints].dist = laser.ranges[i];
                Detection::LatestLaserScan[i-nFilterPoints].angle = -a;
                Detection::LatestLaserScan[i-nFilterPoints].x = sin(-a) * laser.ranges[i];
                Detection::LatestLaserScan[i-nFilterPoints].y = cos(-a) * laser.ranges[i];
            }
            else{ // 30 meters if range is below threshold
                Detection::LatestLaserScan[i-nFilterPoints].dist = 30;
                Detection::LatestLaserScan[i-nFilterPoints].angle = -a;
                Detection::LatestLaserScan[i-nFilterPoints].x = sin(-a) * 30;
                Detection::LatestLaserScan[i-nFilterPoints].y = cos(-a) * 30;
            }

            a += laser.angle_increment;
        }
    }

    //Find 2 lines of the walls in the corridor
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

        // Look for left wall (RL SCAN)
        for(unsigned int i = 700; i < 940; i = i + 20){
            if(LatestLaserScan[i].dist < 1.5){
                if(Detection::lineFit(aFit, bFit, i-nPointsSearch, i) ){
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


        // Look for right wall (RL SCAN)
        for(unsigned int i = 300; i > 50; i = i - 20){
            if(LatestLaserScan[i].dist < 1.5){
                if(Detection::lineFit(aFit, bFit, i, i+nPointsSearch) ){
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

        CorridorWalls walls;
        walls.rightWall1 = right1;
        walls.rightWall2 = right2;
        walls.leftWall1 = left1;
        walls.leftWall2 = left2;
        walls.escaped = !(detectedRight && detectedLeft);
        return walls;
    }

    void Detection::findExitsAndCorners_RL(){
        //std::cout << "TEST" << std::endl;
        int nExits = 20;
        int nCorners = 20;

        for (int l = 0; l < nExits; l=l+1){
            Exits_RL[l].detected = false;
            Corners_RL[l].detected = false;
        }
        //double errorThresh = 0.05; //Amount of deviation from linefit which is allowed (due to sensor noise)
        double errorThresh = 0.05; //[m] Amount of deviation from linefit which is allowed (due to sensor noise)
        int nPointsThresh = 15; //Amount of points larger of smaller than expected before action is being undertaken
        int nPointsThreshEqual = 5;

        int nPointsSearch = 50;

        int iExit1 = 0;
        int iExit2 = 0;

        double aFit = 0; double bFit = 0;

        for (unsigned int i = 0; i < (1000 - 2*15 - nPointsSearch); i = i+1){
            if(lineFit(aFit, bFit, i, i+nPointsSearch)){ //Line found, next: search for deviation in positive direction

                int nDeviations = 0;

                for (int j = i+nPointsSearch; j < 1000 - 2*15; ++j){

                   //std::cout << j << std::endl;
                    double aFit_perp = -1/aFit; //Slope of the line perpendicular to fitted line
                    double bFit_perp = LatestLaserScan[j].y - aFit_perp * LatestLaserScan[j].x;

                    double x_intersect = (bFit_perp - bFit)/(aFit - aFit_perp);
                    double y_intersect = aFit*x_intersect + bFit;

                    double distance_line = sqrt( pow(x_intersect - LatestLaserScan[j].x, 2) + pow(y_intersect - LatestLaserScan[j].y,2)  );

                    //std::cout << distance_line << std::endl;

                    if (distance_line > errorThresh){
                        nDeviations = nDeviations + 1;
                    } else{
                        nDeviations = 0;
                    }


                    if(nDeviations > nPointsThresh && LatestLaserScan[j].dist < sqrt( pow(x_intersect,2) + pow(y_intersect,2) )){

                        //Corner detected
                        if(LatestLaserScan[j-nPointsThresh].dist <= 0.9 * LatestLaserScan[j-nPointsThresh - 1].dist ){
                            i = j;
                            j = 970;
                        }
                        else{
                        Corner corner;
                        corner.cornerPoint = LatestLaserScan[j-nPointsThresh - 1];
                        corner.detected = true;

                        for(int l = 0; l < nCorners; l = l+1){
                            if(Corners_RL[l].detected==false){
                                Corners_RL[l] = corner;
                                break;
                            }
                        }

                        i = j;
                        j = 970; // I know it's ugly, but breaking failed, I deeply apologize for this ugly piece of unscalable code.
                    }
                    }


                    if (nDeviations > nPointsThresh && LatestLaserScan[j].dist > sqrt( pow(x_intersect,2) + pow(y_intersect,2) )){

                        // Exit detected
                        iExit1 = j - nPointsThresh - 2;

                        //Start searching for the second point of the exit
                        int nEqual = 0;

                        for (int k = j; k < 1000 - 2*15 - nPointsSearch; ++k){

                            aFit_perp = -1/aFit; //Slope of the line perpendicular to fitted line
                            bFit_perp = LatestLaserScan[k].y - aFit_perp * LatestLaserScan[k].x;

                            double x_intersect = (bFit_perp - bFit)/(aFit - aFit_perp);
                            double y_intersect = aFit*x_intersect + bFit;

                            double distance_line = sqrt( pow(x_intersect - LatestLaserScan[k].x, 2) + pow(y_intersect - LatestLaserScan[k].y,2)  );


                            if (distance_line < errorThresh){
                                nEqual = nEqual + 1;
                            }
                            else{
                                nEqual = 0;
                            }

                            if (nEqual >= nPointsThreshEqual){
                               iExit2 = k - nPointsThreshEqual +1;
                               i = iExit2;
                               k = 970;
                               j = 970;
                               Exit exit;
                               exit.exitPoint1 = LatestLaserScan[iExit1];
                               exit.exitPoint2 = LatestLaserScan[iExit2];
                               exit.detected = true;
                               for(int l = 0; l < nExits; l = l+1){
                                   if(!Exits_RL[l].detected){
                                       Exits_RL[l] = exit;
                                       break;
                                   }
                               }
                            }
                        }
                    }
                }
            }
        }
    }


    void Detection::findExitsAndCorners_LR(){ // LR SCAN

        int nExits = 20;
        int nCorners = 20;

        for (int l = 0; l < nExits; l=l+1){
            Exits_LR[l].detected = false;
            Corners_LR[l].detected = false;
        }
        double errorThresh = 0.05; //[m] Amount of deviation from linefit which is allowed (due to sensor noise)
        int nPointsThresh = 15; //Amount of points larger of smaller than expected before action is being undertaken
        int nPointsThreshEqual = 5;

        int nPointsSearch = 50;

        int iExit1_LR = 0;
        int iExit2_LR = 0;


        double aFit = 0; double bFit = 0;

        for (unsigned int i = 970; i > 0 + nPointsSearch; i = i-1){

            if(lineFit(aFit, bFit, i-nPointsSearch, i)){ //Line found, next: search for deviation in positive direction

                int nDeviations = 0;

                for (int j = i-nPointsSearch; j > 0; --j){

                    double aFit_perp = -1/aFit; //Slope of the line perpendicular to fitted line
                    double bFit_perp = LatestLaserScan[j].y - aFit_perp * LatestLaserScan[j].x;

                    double x_intersect = (bFit_perp - bFit)/(aFit - aFit_perp);
                    double y_intersect = aFit*x_intersect + bFit;

                    double distance_line = sqrt( pow(x_intersect - LatestLaserScan[j].x, 2) + pow(y_intersect - LatestLaserScan[j].y,2)  );

                    if (distance_line > errorThresh){
                        nDeviations = nDeviations + 1;
                    } else{
                        nDeviations = 0;
                    }


                    if(nDeviations > nPointsThresh && LatestLaserScan[j].dist < sqrt( pow(x_intersect,2) + pow(y_intersect,2) )){

                        //Corner detected
                        if(LatestLaserScan[j+nPointsThresh].dist <= 0.9 * LatestLaserScan[j+nPointsThresh + 1].dist ){
                            i = j;
                            j = 0;
                        }
                        else{
                        Corner_LR corner;
                        corner.cornerPoint_LR = LatestLaserScan[j+nPointsThresh + 1];
                        corner.detected = true;

                        for(int l = 0; l < nCorners; l = l+1){
                            if(Corners_LR[l].detected==false){
                                Corners_LR[l] = corner;
                                break;
                            }
                        }

                        i = j;
                        j = 0; // I know it's ugly, but breaking failed, I deeply apologize for this ugly piece of unscalable code.
                    }
                    }


                    if (nDeviations > nPointsThresh && LatestLaserScan[j].dist > sqrt( pow(x_intersect,2) + pow(y_intersect,2) )){

                        // Exit detected
                        iExit1_LR = j + nPointsThresh + 2;


                        //Start searching for the second point of the exit
                        int nEqual = 0;

                        for (int k = j; k > 0 + nPointsSearch; --k){

                            aFit_perp = -1/aFit; //Slope of the line perpendicular to fitted line
                            bFit_perp = LatestLaserScan[k].y - aFit_perp * LatestLaserScan[k].x;

                            double x_intersect = (bFit_perp - bFit)/(aFit - aFit_perp);
                            double y_intersect = aFit*x_intersect + bFit;

                            double distance_line = sqrt( pow(x_intersect - LatestLaserScan[k].x, 2) + pow(y_intersect - LatestLaserScan[k].y,2)  );


                            if (distance_line < errorThresh){
                                nEqual = nEqual + 1;
                            }
                            else{
                                nEqual = 0;
                            }

                            if (nEqual >= nPointsThreshEqual){
                               iExit2_LR = k + nPointsThreshEqual -1;
                               i = iExit2_LR;
                               k = 0;
                               j = 0;
                               Exit_LR exit;
                               exit.exitPoint1_LR = LatestLaserScan[iExit1_LR];
                               exit.exitPoint2_LR = LatestLaserScan[iExit2_LR];
                               exit.detected = true;
                               for(int l = 0; l < nExits; l = l+1){
                                   if(!Exits_LR[l].detected){
                                       Exits_LR[l] = exit;
                                       break;
                                   }
                               }
                            }
                        }
                    }
                }
            }
        }
    }


    double Detection::distance_to_front(){
        double dist_total;
        for(int i = 450; i < 500; i = i+1 ){
            dist_total = dist_total = LatestLaserScan[i].dist;
        }
        double distance = dist_total/50;

        return distance;
    }


    //Fit a line y = a*x + b between a set of specified points (LatestLaserScan points between index firstPoint and lastPoint)

    bool Detection::lineFit(double &aFit, double &bFit, int firstPoint, int lastPoint){
        unsigned int nFitPoints = 10; //Amount of points that are used to fit line


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
        }

        //Fit line y = a*x + b through the first and last (averaged) point

        aFit = (lineFitPoint[nFitPoints-1].y - lineFitPoint[0].y)/(lineFitPoint[nFitPoints-1].x - lineFitPoint[0].x);
        bFit = lineFitPoint[0].y - aFit*lineFitPoint[0].x;

        double fitError = 0;

        for(unsigned int i = 1; i < nFitPoints - 1; ++i){
            double aFit_perp = -1/aFit; //Slope of the line perpendicular to fitted line
            double bFit_perp = lineFitPoint[i].y - aFit_perp * lineFitPoint[i].x;

            double x_intersect = (bFit_perp - bFit)/(aFit - aFit_perp);
            double y_intersect = aFit*x_intersect + bFit;

            fitError = fitError = sqrt( pow(x_intersect - lineFitPoint[i].x, 2) + pow(y_intersect - lineFitPoint[i].y,2)  );
        }

        double RMSerror = sqrt(fitError);

        //Total threshold for linefit (larger when more points are evaluated or when points are further apart from each other)

        double errorThresh = LINEFITTHRESH*(lastPoint-firstPoint)*(nFitPoints - 2);

        if (RMSerror < errorThresh){
            aFitPlot = aFit; //Save data globally for visualization.
            bFitPlot = bFit; //Save data globally for visualization.

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
