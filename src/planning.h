#ifndef planning_H
#define planning_H


// Furthest point
typedef struct {
    int x;
    int y;
    double angle;
    double dist;
} Furthest_point;


// Store exit data: two corners, angles and a flag whether detected
typedef struct {
    bool detected;
    int x1;
    int y1;
    int x2;
    int y2;
    double angle1;
    double angle2;
} Exit;


// Destination that is passed to the Control block
typedef struct {
    int x;
    int y;
    int angle;
} Destination;


#endif
