#include "stateMachine.h"

bool state_machine(WorldModel * worldModel){

    // World model should be read for:
    // 1) number of rooms detected, number of rooms entered
    // 2) current location (flag with room or corridor)
    // 3) properties of the current room (exits, corners, including empty fields
    //                                        (e.g. unidentified corners))
    // 4) knowledge about the exit that we have entered from
    // 5) nearest distance to the wall

    std::cout << "works till here" << "\n";

    bool end_of_program=false;
    High_State high_st = worldModel->get_currentHighState();
    Low_State low_st = worldModel->get_currentLowState();


    //---------------------------------------//
    //                                       //
    //     to be assigned from world model   //
    //                                       //
    //---------------------------------------//

    int numb_rooms_in_corridor = worldModel->get_connectedRooms(0).size();    // total number of rooms detected
    int numb_rooms_explored = worldModel->get_enteredRooms();
    int numb_nexted_exits = worldModel->get_nestedExits();      // number of total exits in rooms - 1*numb_rooms_explored

    Location location = worldModel->get_currentLocation();

    // second phase
    bool object_found = false;
    bool near_object = false;



    Room current_room = worldModel->get_curRoom();
    int current_room_number = current_room.roomID;


    std::cout << "works till here" << "\n";

    int numb_corners_detected = 0;
    int numb_exits = 0;

    if (location == IN_ROOM){
        numb_corners_detected = current_room.corners.size();
        numb_exits = worldModel->get_explorationStack().size();// assigned from the world model
    }


    bool end_of_corridor = false;       // have PICO reached the end of the corridor once?
    bool at_start = false;              // have we returned back to the initial condition yet?

    if (location == IN_CORRIDOR){
        Point_map pos = worldModel->get_globalPosition();
        Point_map corridorCorner1 = current_room.corners[0], corridorCorner2 = current_room.corners[1];

        double dist1, dist2;

        dist1 = sqrt(pow(corridorCorner1.x - pos.x , 2) + pow(corridorCorner1.y - pos.y , 2));
        dist2 = sqrt(pow(corridorCorner2.x - pos.x , 2) + pow(corridorCorner2.y - pos.y , 2));

        if (dist1 < DIST_SETPOINT || dist2 <DIST_SETPOINT){
            end_of_corridor = true;
        }

        if (sqrt(pow(pos.x,2)+pow(pos.y,2)) < DIST_SETPOINT) {
            at_start = true;
        }

    }


    //------------end of assigning variables-------------//




    switch(high_st) {
    case EXPLORE_HOSPITAL:

        std::cout << "EXPLORING THE HOSPITAL " << std::endl;


        if (location == IN_CORRIDOR) {

//            std::cout << "In corridor" << "\n";

            switch(low_st){

            case EXPLORE_CORRIDOR:

                if (numb_rooms_in_corridor == 0 || !end_of_corridor){
//                    std::cout << "Exploring the corridor " << std::endl;

                } else if (numb_rooms_in_corridor > numb_rooms_explored - numb_nexted_exits) {
                    worldModel->set_currentLowState(GO_TO_NEXT_ROOM);
                    std::cout << "Moving to the next room" << "\n";
                }
                break;

            case GO_TO_NEXT_ROOM:

                if (worldModel->get_destination().dist < DIST_SETPOINT){
                    worldModel->set_currentLowState(GO_INSIDE_ROOM);
                    std::cout << "Entering a room from corridor" << "\n";
                } else {
//                    std::cout << "Moving to the next room" << "\n";
                }
                break;

            case GO_INSIDE_ROOM:
                worldModel->set_currentLocation(IN_ROOM);
                worldModel->set_currentLowState(EXPLORE_ROOM);
                std::cout << "Exploring the room " << std::endl;
                break;
            }

            // command to go to  first exit
        } else if (location == IN_ROOM) {

            switch(low_st){

            case EXPLORE_ROOM:

//                std::cout << "Exploring the room " << std::endl;

                // check if all the corners are detected
                if (numb_corners_detected == 4){

                    // if nested exit detected
                    if (numb_exits > 1) {
                        worldModel->set_currentLowState(GO_TO_NEXT_ROOM);
                        worldModel->set_closestRoom();
                        std::cout << "Going to the nested room " << std::endl;
                    }
                    else {
                        low_st = EXIT_TO_PREV_ROOM;
                        std::cout << "Exiting to the previous room " << std::endl;
                    }
                }

                break;

            case GO_INSIDE_ROOM:
                if (current_room.previousRoom == 0) {
                    worldModel->set_currentLowState(GO_TO_NEXT_ROOM);
                    worldModel->set_currentLocation(IN_CORRIDOR);
                    worldModel->set_closestRoom();
                    worldModel->set_currentLocation(location);
                    std::cout << "Going into the corridor " << std::endl;
                    worldModel->update_JSON();
                } else {
                    worldModel->set_currentLowState(EXPLORE_ROOM);
                }
                break;

            case GO_TO_NEXT_ROOM:

                if (worldModel->get_destination().dist < DIST_SETPOINT){
                    worldModel->set_currentLowState(GO_INSIDE_ROOM);
                    std::cout << "Entering a nested room" << "\n";
                } else {
//                    std::cout << "Going to nested room " << std::endl;
                }
                break;

            case EXIT_TO_PREV_ROOM:
                if (worldModel->get_destination().dist < DIST_SETPOINT){
                    worldModel->set_currentLowState(GO_INSIDE_ROOM);
                    std::cout << "Exiting to lower order room" << "\n";
                } else {
//                    std::cout << "Leaving the room" << "\n";
                }
                break;
            }
        }


        if (numb_rooms_explored == (numb_nexted_exits + numb_rooms_explored)) {
            std::cout << "Moving to starting point " << std::endl;
            worldModel->set_currentHighState(RETURN_TO_INIT);
            worldModel->set_currentLowState(GO_TO_START);
        }

        break;

    case RETURN_TO_INIT:
        // assuming we are in the corridor
//        std::cout << "RETURNING TO INITIAL POSITION " << std::endl;


        switch(low_st) {

        case GO_TO_START:
            if (at_start){
                worldModel->set_currentLowState(PARKING);
                std::cout << "Parking " << std::endl;
            } else {
//                std::cout << "Moving to starting point " << std::endl;
            }
            break;

        case PARKING:
            std::cout << "Parked! " << std::endl;
            worldModel->set_mostNestedRoom();
            end_of_program = true;
            break;
        }

        break;


        //not executed currently
    case GO_TO_ROOM:
        std::cout << "FINDING OBJECT PHASE " << std::endl;
        switch(low_st) {

        case EXIT_CORRIDOR:
            if (location == IN_CORRIDOR){
//                std::cout << "Exiting corridor " << std::endl;
            } else if (location == IN_ROOM){
                worldModel->set_currentLowState(GO_INSIDE_ROOM);
                std::cout << "Entering the room " << std::endl;
            } else{
                std::cout << "ERROR exiting corridor " << std::endl;
            }
            break;

        case GO_INSIDE_ROOM:
            worldModel->set_nextRoom();
            worldModel->set_currentLowState(GO_TO_NEXT_ROOM);
            std::cout << "Going to next room " << std::endl;
            break;

        case GO_TO_NEXT_ROOM:

            if (object_found){
                worldModel->set_currentLowState(STAND_NEXT_TO_OBJECT);
            } else {
                if (worldModel->get_destination().dist < DIST_SETPOINT){
                    worldModel->set_currentLowState(GO_INSIDE_ROOM);
                    std::cout << "Entering the room " << std::endl;
                } else {
//                    std::cout << "Going to next room " << std::endl;
                }
            }
            break;

        case STAND_NEXT_TO_OBJECT:
//            std::cout << "Moving to the object " << std::endl;
            if (near_object){
                end_of_program = true;
                std::cout << "Near the object " << std::endl;
            }
            break;
        }

        break;
    }


    return end_of_program;
}
