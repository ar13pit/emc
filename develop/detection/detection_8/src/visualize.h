#ifndef VISUALIZE
#define VISUALIZE

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"

class Visualizer
{
//public:
//Visualizer(){
//    return;
//}
public:
void init_visualize();
void plot_xy_color(double, double, double, double, double);
void publish();
void plotExit(Exit);
void plotCorner(Corner);
void plotExit_LR(Exit_LR);
void plotCorner_LR(Corner_LR);
void plotLine(double, double, Point, Point);
cv::Point2d canvas_center;

cv::Point2d worldToCanvas(double, double);

cv::Mat canvas;
};
#endif // VISUALIZE


