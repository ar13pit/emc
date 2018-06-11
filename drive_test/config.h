#define EXECUTION_RATE 10 // [Hz]
#define FORWARD_SPEED  0.5 // [m/s]
#define ROTATE_SPEED   -0.7 // [rad/s]
#define DIST_BACKWARDS 0.1 // [m]
#define SETPOINT_CORRIDOR   0.25            // [m]

#define MIN_DIST_TO_WALL 0.75 // [m]
#define MAX_RANGE_LRF 30.0 //[m]
#define LRF_THRESH 0.05 //[m]

#define LINEFITTHRESH 0.00005 // Higher --> more chance of finding a wall
#define TOL  0.01	// tolerance for calculations

#define TURN_MARGIN 0.01*M_PI // [rad/s]
#define TURN_COMPLETE 0.05 // [rad/s] - margin for angle completion
#define FRW_COMPLETE 0.05 // [m] - margin for distance competion
#define THRESHOLD_CORRIDOR 0.5
#define EXIT_THRESHOLD 0.5 //[m] - distance at which to switch to corridor escape phase
#define DIST_SETPOINT 0.02 // [m] - distnace between the setpoints
