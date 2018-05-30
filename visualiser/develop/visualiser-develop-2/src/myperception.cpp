#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <emc/io.h>
#include <string>
#include <sstream>
#include <iostream>
#include <emc/rate.h>

#include "driveControl.h"
#include "detection.h"
#include "worldModel.h"
#include "planning.h"
#include "config.h"

using namespace cv;

double resolution = 0.01;
cv::Point2d canvas_center;

cv::Point2d worldToCanvas(double x, double y)
{
    return cv::Point2d(-y / resolution, -x / resolution) + canvas_center;
}

int show_canvas(emc::LaserData scan){

emc::IO io;
emc::Rate r(30);

Detection_data data;


// Create canvas

while(io.ok())
{
    Mat canvas = cv::Mat(500, 500, CV_8UC3, cv::Scalar(50, 50, 50));
    canvas_center = cv::Point2d(canvas.rows / 2, canvas.cols / 2);

    // Draw PICO
    cv::Scalar robot_color(0, 0, 255);
    std::vector<std::pair<double, double> > robot_points;
    robot_points.push_back(std::pair<double, double>( 0.1,  -0.2));
    robot_points.push_back(std::pair<double, double>( 0.1,  -0.1));
    robot_points.push_back(std::pair<double, double>( 0.05, -0.1));
    robot_points.push_back(std::pair<double, double>( 0.05,  0.1));
    robot_points.push_back(std::pair<double, double>( 0.1,   0.1));
    robot_points.push_back(std::pair<double, double>( 0.1,   0.2));
    robot_points.push_back(std::pair<double, double>(-0.1,   0.2));
    robot_points.push_back(std::pair<double, double>(-0.1,  -0.2));

    for(unsigned int i = 0; i < robot_points.size(); ++i)
    {
        unsigned int j = (i + 1);  robot_points.size();
        cv::Point2d p1 = worldToCanvas(robot_points[i].first, robot_points[i].second);
        cv::Point2d p2 = worldToCanvas(robot_points[j].first, robot_points[j].second);
        cv::line(canvas, p1, p2, robot_color, 2);
    }

emc::LaserData scan;
if(!io.readLaserData(scan))
        continue;

    // Draw laser data
    double a = scan.angle_min;
    for(unsigned int i = 0; i < scan.ranges.size(); ++i)
    {
        double x = cos(a) * scan.ranges[i];
        double y = sin(a) * scan.ranges[i];

        cv::Point2d p = worldToCanvas(x,y);
        if (p.x >= 0 && p.y >= 0 && p.x < canvas.cols && p.y < canvas.rows)
            canvas.at<cv::Vec3b>(p) = cv::Vec3b(255, 255, 255);

        a += scan.angle_increment;
    }

//    Furthest Point
//    cv::Point2d

//            Point Detection::findFurthestPoint(){
//                int imax = 0;
//                double max = 0;
//                for(int i = 0; i < 1000-2*15; ++i){
//                    if (LatestLaserScan[i].dist > max){
//                        max = LatestLaserScan[i].dist;
//                        imax = i;
//                    }
//                }
//                return LatestLaserScan[imax];
//            }


    // Corridor lines
    cv::Point2d Wall_left_1 = worldToCanvas(data.corridor.leftWall1.x, data.corridor.leftWall1.y);
    cv::Point2d Wall_left_2 = worldToCanvas(data.corridor.leftWall2.x, data.corridor.leftWall2.x);
    cv::Point2d Wall_right_1 = worldToCanvas(data.corridor.rightWall1.x, data.corridor.rightWall1.y);
    cv::Point2d Wall_right_2 = worldToCanvas(data.corridor.rightWall2.x, data.corridor.rightWall2.y);

    line(canvas, Wall_left_1, Wall_left_2, Scalar(255,255,255),2,8);
    line(canvas, Wall_right_1, Wall_right_2, Scalar(255,255,255),2,8);


//    Corner coordinates

//    Exit coordinates


//    Set Point
//    cv::Point2d setpoint = worldToCanvas(dest); // input x,y coordinates from planning
//    circle(canvas,setpoint,5,Scalar(0,255,255),5,8,0);
//    putText(canvas,"set Point",cv::Point2d(25,85),FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,255,255));

    // Show legend
    putText(canvas,"Pico", cv::Point2d(25,25), FONT_HERSHEY_SIMPLEX, 0.5, Scalar( 0, 0, 255 ));
    putText(canvas,"Laser", cv::Point2d(25,45), FONT_HERSHEY_SIMPLEX, 0.5, Scalar( 255, 255, 255 ));

    // Show canvas
    imshow("PICO Visualisation", canvas);
    waitKey(3);
    r.sleep();
 }
   return(0);
}
