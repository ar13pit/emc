#include "monitoring.h"

/* EXPLORE_CORRIDOR,   // for initial phase to count exits in the corridor
EXIT_CORRIDOR,
GO_TO_START,        // after all rooms are located
PARKING,            // park backwards

EXPLORE_ROOM,
GO_TO_NEXT_ROOM,
EXIT,
EXIT_MAIN_ROOM,      // when returning from the nested room to the main room, don't explore it

STAND_NEXT_TO_OBJECT */

// explore the rooms in the hospital
bool explore_hospital(WorldModel * worldModel){
    Low_State current_low = worldModel->get_current_low_state();

    // room-related low controllers
    if (current_low == EXIT) {

    } else if (current_low == EXPLORE_ROOM){

    } else if (current_low == GO_TO_NEXT_ROOM){

    } else if (current_low == EXIT_MAIN_ROOM){

    }

    // corridor-related low controls
    if (current_low == EXPLORE_CORRIDOR){

    } else if (current_low == EXIT_CORRIDOR){

    }

}


// return to initial point
bool return_to_init(WorldModel * worldModel){
    Low_State current_low = worldModel->get_current_low_state();

    if (current_low == GO_TO_START){

    } else if (current_low == PARKING) {

    }
}

// main function in monitoring
bool monitoring(WorldModel * worldModel){

    High_State current_high = worldModel->get_current_high_state();

    if (current_high == TOO_CLOSE_TO_WALL){

        bool wall_push_back(worldModel);

    } else {

        if (current_high == EXPLORE_HOSPITAL){

            bool explore_hospital(worldModel);

        } else if (current_high == RETURN_TO_INIT) {

            bool return_to_init(worldModel);
        } else {
            std::cout << "Error in monitoring function" <<"\n";
        }
    }
}
