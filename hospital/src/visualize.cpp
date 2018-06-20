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


using namespace std;
//using namespace cv;




double resolution = 0.01;
//double a;


//int show_canvas(emc::LaserData scan)
void Visualizer::init_visualize()
{

    cv::Scalar robot_color(0, 0, 255);
    canvas = cv::Mat(550, 800, CV_8UC3, cv::Scalar(50, 50, 50)); // Change first two entries to adjust visualiser resolution (1100,1100) #GODDAMNIT HIZAR

    canvas_center = cv::Point2d(canvas.rows / 2, canvas.cols / 2);


    std::vector<std::pair<double, double> > robot_points;
    robot_points.push_back(std::pair<double, double>(  0.2,  0.1));
    robot_points.push_back(std::pair<double, double>(  0.1,  0.1));
    robot_points.push_back(std::pair<double, double>(  0.1,  0.05));
    robot_points.push_back(std::pair<double, double>( -0.1,  0.05));
    robot_points.push_back(std::pair<double, double>( -0.1,  0.1));
    robot_points.push_back(std::pair<double, double>( -0.2,  0.1));
    robot_points.push_back(std::pair<double, double>( -0.2, -0.1));
    robot_points.push_back(std::pair<double, double>(  0.2, -0.1));

    for(unsigned int i = 0; i < robot_points.size(); ++i)
    {
        unsigned int j = (i + 1) % robot_points.size();
        cv::Point2d p1 = worldToCanvas(robot_points[i].first, robot_points[i].second);
        cv::Point2d p2 = worldToCanvas(robot_points[j].first, robot_points[j].second);
        cv::line(canvas, p1, p2, robot_color, 2);
    }

    // wall avoidance perimeter
    double bump_avoidance = 0.3; // adjust avoidance distance (stay away from wall param.)
    cv::Point2d pico_center = worldToCanvas(0, 0);
    circle(canvas, pico_center, bump_avoidance/resolution, cv::Scalar(125,125,125),2,8,0);
    //circle(exi)

    // Legend
    putText(canvas,"PICO", cv::Point2d(25,25), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar( 0, 0, 255));
    putText(canvas,"LRF", cv::Point2d(25,45), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(60, 80, 200));

    //}
}

/*    // wall avoidance perimeter
    double bump_avoidance = 0.5; // adjust avoidance distance (stay away from wall param.)
    cv::Point2d pico_center = worldToCanvas(0, 0);
    circle(canvas, pico_center, bump_avoidance/resolution, Scalar(125,125,125),2,8,0);
    //circle(exi)

    // Legend
    putText(canvas,"PICO", cv::Point2d(25,25), FONT_HERSHEY_SIMPLEX, 0.5, Scalar( 0, 0, 255));
    putText(canvas,"LRF", cv::Point2d(25,45), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));

    // Canvas
    cv::imshow("PICO Visualisation", canvas);
    //cv::waitKey(3);

}


*/
/*void Visualizer::LaserData_visualize(){
    //emc::IO io;
    //emc::Rate r(30);

    //scan = Detection::LatestLaserScan.length

        //double a = scan.angle_min;
        for(unsigned int i = 0; i < Detection::LatestLaserScan.size(); ++i)
        {
            std::cout << "UPDATE VISUALIZE";

            double x = Detection::LatestLaserScan[i].x;
            double y = Detection::LatestLaserScan[i].y;

            //x = 1; y = 1; a = 1;

            cv::Point2d p = worldToCanvas(x, y);
            if (p.x >= 0 && p.y >= 0 && p.x < canvas.cols && p.y < canvas.rows)
                canvas.at<cv::Vec3b>(p) = cv::Vec3b(0, 255, 0);

            //a += scan.angle_increment;
        }

        // Canvas
        cv::imshow("PICO Visualisation", canvas);
        cv::waitKey(3);

}*/

void Visualizer::plot_xy_color(double x, double y, double R, double G, double B){
    cv::Point2d p = worldToCanvas(x, y);
    if (p.x >= 0 && p.y >= 0 && p.x < canvas.cols && p.y < canvas.rows)
        canvas.at<cv::Vec3b>(p) = cv::Vec3b(R, G, B);
}

void Visualizer::publish(){
    cv::imshow("PICO Visualisation", canvas);
    cv::waitKey(3);
}

void Visualizer::plotExit(Exit exit){
    if(exit.detected){
        cv::Point2d exit1 = worldToCanvas(exit.exitPoint1.x, exit.exitPoint1.y);
        cv::Point2d exit2 = worldToCanvas(exit.exitPoint2.x, exit.exitPoint2.y);

        Visualizer::plot_xy_color(exit.exitPoint2.x, exit.exitPoint2.y,0,255,0);
        circle(canvas, exit1, 1, cv::Scalar(0,255,0),5,8,0);
        circle(canvas, exit2, 1, cv::Scalar(0,255,0),5,8,0);

    }
}

void Visualizer::plotCorner(Corner corner){
    if(corner.detected){
        cv::Point2d corner1 = worldToCanvas(corner.cornerPoint.x, corner.cornerPoint.y);

        Visualizer::plot_xy_color(corner.cornerPoint.x, corner.cornerPoint.y,0,255,0);
        circle(canvas, corner1, 1, cv::Scalar(0,0,255),5,8,0);

    }
}


void Visualizer::plotExit_LR(Exit_LR exit){
    if(exit.detected){
        cv::Point2d exit1_LR = worldToCanvas(exit.exitPoint1_LR.x, exit.exitPoint1_LR.y);
        cv::Point2d exit2_LR = worldToCanvas(exit.exitPoint2_LR.x, exit.exitPoint2_LR.y);

        Visualizer::plot_xy_color(exit.exitPoint2_LR.x, exit.exitPoint2_LR.y,0,255,0);
        circle(canvas, exit1_LR, 1, cv::Scalar(255,255,0),5,8,0);
        circle(canvas, exit2_LR, 1, cv::Scalar(255,255,0),5,8,0);

    }
}

void Visualizer::plotCorner_LR(Corner_LR corner){
    if(corner.detected){
        cv::Point2d corner1_LR = worldToCanvas(corner.cornerPoint_LR.x, corner.cornerPoint_LR.y);

        Visualizer::plot_xy_color(corner.cornerPoint_LR.x, corner.cornerPoint_LR.y,0,255,0);
        circle(canvas, corner1_LR, 1, cv::Scalar(0,255,255),5,8,0);

    }
}

void Visualizer::plotLine(double aFit, double bFit, Point firstPoint, Point lastPoint){
    double x1 = -10;
    double x2 = 10;
    double y1 = aFit * x1 + bFit;
    double y2 = aFit * x2 + bFit;

    cv::Point2d line1 = worldToCanvas(x1, y1);
    cv::Point2d line2 = worldToCanvas(x2, y2);

    line(canvas,line1,line2,cv::Scalar(255,0,0),1,cv::LINE_8,0);
}

cv::Point2d Visualizer::worldToCanvas(double x, double y){
    return cv::Point2d(x / resolution, -y / resolution) + canvas_center;
}
