#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <emc/io.h>
#include <string>
#include <sstream>
#include <iostream>

emc::LaserData scan;

using namespace cv;


double resolution = 0.01;
cv::Point2d canvas_center;

cv::Point2d worldToCanvas(double x, double y)
{
    return cv::Point2d(-y / resolution, -x / resolution) + canvas_center;
}

int main()
{
int show_canvas(emc::LaserData scan); {

    // Create canvas
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

    // Show legend
    putText(canvas,"Pico", cv::Point2d(25,25), FONT_HERSHEY_SIMPLEX, 0.5, Scalar( 0, 0, 255 ));
    putText(canvas,"Input", cv::Point2d(25,70), FONT_HERSHEY_SIMPLEX, 0.5, Scalar( 255, 255, 255 ));

    // Show canvas
    imshow("PICO Visualisation", canvas);
    waitKey(0);
 }
   return(0);
}
