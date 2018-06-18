#include "helper.h"

Point::Point () : x(0), y(0), angle_(0), dist_(0) {};

void Point::angle (double newAngle) {
    angle_ = newAngle;
};

double Point::angle () {
    return angle_;
};

double Point::dist () {
    return dist_;
};

void Point::dist (double newDistance) {
    dist_ = newDistance;
};


Exit::Exit () {};

void Exit::detected (bool exitDetectedFlag) {
    detected_ = exitDetectedFlag;
};

bool Exit::detected () {
    return detected_;
};
