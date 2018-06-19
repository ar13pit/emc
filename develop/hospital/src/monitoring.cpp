#include "monitoring.hpp"

// explore the rooms in the hospital
bool explore_hospital(WorldModel * worldModel){
    std::cout << "Explore" << std::endl;

    Room room;
    Point dest;
    Point navigateTo;
    Low_State current_low = worldModel->get_currentLowState();

    Planning planning(worldModel);

    if (current_low == GO_INSIDE_ROOM) {


        // to be filled in


    } else if (current_low == EXPLORE_ROOM){

        room = worldModel->get_currentRoom();
        dest = planning.driveInRoom();

    } else if (current_low == GO_TO_NEXT_ROOM){

        room = worldModel->get_closestRoom();
        navigateTo = planning.getNearbyExitPoint(room);
        dest = planning.driveToPoint(navigateTo);

    } else if (current_low == EXPLORE_CORRIDOR){

        dest = planning.setpointInCorridor();

    } else if (current_low == EXIT_TO_PREV_ROOM){

        room = worldModel->get_currentRoom();
        navigateTo = planning.getNearbyExitPoint(room);
        dest = planning.driveToPoint(navigateTo);
    }

    worldModel->set_destination(dest);
    return true;
}


// return to initial point
bool return_to_init(WorldModel * worldModel){

    Point dest;
    Point navigateTo;
    Low_State current_low = worldModel->get_currentLowState();

    Planning planning(worldModel);

    if (current_low == GO_TO_START){

        navigateTo = planning.getStartPos();
        dest = planning.driveToPoint(navigateTo);

    } else if (current_low == PARKING) {

        dest = planning.parkPico();

    }

    worldModel->set_destination(dest);
    return true;
}

bool search_for_object(WorldModel * worldModel){
    Point dest;
    Point navigateTo;
    Low_State current_low = worldModel->get_currentLowState();

    // if (current_low == EXIT_CORRIDOR){
        // to be filled in
    if(current_low == GO_TO_NEXT_ROOM){
        // to be filled in
    } else if(current_low == GO_INSIDE_ROOM){
        // to be filled in
    } else if (current_low == STAND_NEXT_TO_OBJECT){
        // to be filled in
    }
}

// main function in monitoring
bool monitoring(WorldModel * worldModel){

    High_State current_high = worldModel->get_currentHighState();

    if (current_high == EXPLORE_HOSPITAL){

        explore_hospital(worldModel);

    } else if (current_high == RETURN_TO_INIT) {

        return_to_init(worldModel);

    } else if (current_high == GO_TO_ROOM){

        search_for_object(worldModel);

    } else {
        std::cout << "Error in monitoring function" <<"\n";
    }

    return true;
}
