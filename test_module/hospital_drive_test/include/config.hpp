#ifndef CONFIG_H
#define CONFIG_H

#define EXECUTION_RATE 10                           // [Hz]
#define DRIVE_RATE 40                               // [Hz]
#define FORWARD_SPEED  0.5                          // [m/s]
#define ROTATE_SPEED   -0.7                         // [rad/s]
#define DIST_BACKWARDS 0.1                          // [m]
#define SETPOINT_CORRIDOR   0.25                    // [m]

#define MIN_DIST_TO_WALL 0.3                        // [m]
#define MAX_RANGE_LRF 30.0                          // [m]
#define LRF_THRESH 0.05                             // [m]

#define LINEFITTHRESH 0.00005                       // Higher --> more chance of finding a wall
#define TOL 0.01                                    // tolerance for calculations

#define TURN_MARGIN 0.01*M_PI                       // [rad]
#define TURN_COMPLETE 0.03                          // [rad] - margin for angle completion
#define FRW_COMPLETE 0.01                           // [m] - margin for distance competion
#define THRESHOLD_CORRIDOR 0.5
#define DIST_SETPOINT 0.2                           // [m] - distnace between the setpoints

#define MOVESIDEWAYS_FACTOR 0.9*TURN_MARGIN         // [-] Move sideways when a wall is at the side of PICO
#define TURN_WHEN_OBJECT_IN_FRONT 0.5*M_PI          // [rad] Turn when there is a wall at the front of PICO
#define WAIT_BEFORE_EXIT 0.1                        // [m] PICO drives the exit and waits in front of it (at a certain distance)
#define DISTANCE_FROM_WALL_IN_ROOM 1                // [m] When a wall is in front of Pico at a certain distance Pico turns around
#define FRONTWALL_ANGLE 0.1*M_PI                    // [rad] A wall is in front of Pico is it is at a certain abs ange
#define MAPPING_UNCERTAINTY 0.3
#define CORRIDOR_EXIT_WIDTH 1

#define VISUALIZER_RESOLUTION 0.01
#define VISUALIZER_BUMP_AVOIDANCE 0.3
#define JSON_PATH "Rooms.json"                      // Json path



#endif
