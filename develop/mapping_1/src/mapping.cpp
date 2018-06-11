#include <emc/io.h>
#include <emc/rate.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"

#include <string>
#include <sstream>
#include <iostream>

#include <emc/odom.h>
#include <cmath>
#include <cstdlib>

//#include "driveControl.h"
#include "detection.h"
//#include "worldModel.h"
//#include "planning.h"
#include "config.h"
#include "visualize.h"
//#include "main.pp"
#include "mapping.h"


using namespace std;


//int show_canvas(emc::LaserData scan)
void Mapping::init_map()
{
    global_pos.x = 0;
    global_pos.y = 0;
    global_pos.angle = 0;



    odom_diff.x = 0;
    odom_diff.y = 0;
    odom_diff.angle = 0;

    map.clear();


}

void Mapping::update_global_pos(){
    global_pos.x = odom_diff.x + latest_odom.x;
    global_pos.y = odom_diff.y + latest_odom.y;
    global_pos.angle = odom_diff.angle + latest_odom.angle;
}

void Mapping::update_Odometry(){
    while(!inOut->readOdometryData(odom));
}

void Mapping::delta_Odometry(){
    odom_diff.x = global_pos.x - latest_odom.x;
    odom_diff.y = global_pos.y - latest_odom.y;
    odom_diff.angle = global_pos.angle - latest_odom.angle;
}









