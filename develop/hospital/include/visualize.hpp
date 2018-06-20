#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include <emc/io.h>
#include <emc/rate.h>
#include <emc/odom.h>
#include <cmath>
#include <cstdlib>


#include <string>
#include <sstream>
#include <iostream>


//#include "driveControl.h"
#include "helper.hpp"
//#include "worldModel.h"
//#include "planning.h"
#include "config.hpp"

#ifndef VISUALIZE
#define VISUALIZE

class Visualizer {
    cv::Point2d canvas_center;
    cv::Mat canvas;

public:
    // Visualizer();

    void init_visualize();
    void plot_xy_color(double, double, double, double, double);
    void publish();
    void plotExit(Exit);
    void plotCorner(Corner);
    // void plotExit_LR(Exit_LR);
    // void plotCorner_LR(Corner_LR);
    void plotLine(double, double, Point, Point);

    cv::Point2d worldToCanvas(double, double);

};
#endif // VISUALIZE
