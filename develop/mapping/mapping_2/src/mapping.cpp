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
//using namespace cv;




//double a;


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

}









