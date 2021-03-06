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

#include "driveControl.h"
#include "detection.h"
#include "worldModel.h"
#include "planning.h"
#include "config.h"
#include "visualize.h"
#include "main2.hpp"


using namespace std;
using namespace cv;




double resolution = 0.01;
cv::Point2d canvas_center;


cv::Point2d worldToCanvas(double x, double y)
{
    return cv::Point2d(-y / resolution, -x / resolution) + canvas_center;
}

//int show_canvas(emc::LaserData scan)
visualize::visualize()
{
    emc::IO io;
    emc::Rate r(30);

    while(io.ok())
    {
        cv::Mat canvas(500, 500, CV_8UC3, cv::Scalar(50, 50, 50));
        canvas_center = cv::Point2d(canvas.rows / 2, canvas.cols / 2);

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
            unsigned int j = (i + 1) % robot_points.size();
            cv::Point2d p1 = worldToCanvas(robot_points[i].first, robot_points[i].second);
            cv::Point2d p2 = worldToCanvas(robot_points[j].first, robot_points[j].second);
            cv::line(canvas, p1, p2, robot_color, 2);
        }

        emc::LaserData scan;
        if (!io.readLaserData(scan))
            continue;

        double a = scan.angle_min;
        for(unsigned int i = 0; i < scan.ranges.size(); ++i)
        {
            double x = cos(a) * scan.ranges[i];
            double y = sin(a) * scan.ranges[i];

            cv::Point2d p = worldToCanvas(x, y);
            if (p.x >= 0 && p.y >= 0 && p.x < canvas.cols && p.y < canvas.rows)
                canvas.at<cv::Vec3b>(p) = cv::Vec3b(0, 255, 0);

            a += scan.angle_increment;
        }

        PointCorridor corr;
        // set point

        cv::Point2d setpoint = worldToCanvas(corr.get_x(), corr.get_y());
        putText(canvas, "Setpoint", cv::Point2d(25,85), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,255));


        Exit exit;
        // exit points
        cv::Point2d exit_1 = worldToCanvas(exit.exitPoint_det1.x, exit.exitPoint_det1.y);
        

        // corridor detection



        PointCorridor pointcorridor;
        Corridor lines;
        cv::Point2d Wall_left_1 = worldToCanvas(pointcorridor.get_x(), pointcorridor.get_y());
//        cv::Point2d Wall_left_2 = worldToCanvas(data_.corridor.leftWall2.x, data_.corridor.leftWall2.x);
//        cv::Point2d Wall_right_1 = worldToCanvas(data_.corridor.rightWall1.x, data_.corridor.rightWall1.y);
//        cv::Point2d Wall_right_2 = worldToCanvas(data_.corridor.rightWall2.x, data_.corridor.rightWall2.y);


        // wall avoidance perimeter
        double bump_avoidance = 0.5; // adjust avoidance distance (stay away from wall param.)
        cv::Point2d pico_center = worldToCanvas(0, 0);
        circle(canvas, pico_center, bump_avoidance/resolution, Scalar(125,125,125),2,8,0);

        // Legend
        putText(canvas,"PICO", cv::Point2d(25,25), FONT_HERSHEY_SIMPLEX, 0.5, Scalar( 0, 0, 255));
        putText(canvas,"LRF", cv::Point2d(25,45), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));

        // Canvas
        cv::imshow("PICO Visualisation", canvas);
        cv::waitKey(3);

        r.sleep();
    }

}


