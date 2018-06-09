#include "stateMachine.h"

bool state_machine(struct High_state high_st,struct Low_state low_st, WorldModel * worldModel){

    // World model should be read for:
    // 1) number of rooms detected, number of rooms entered
    // 2) current location (flag with room or corridor)
    // 3) properties of the current room (exits, corners, including empty fields
    //                                        (e.g. unidentified corners))
    // 4) knowledge about the exit that we have entered from
    // 5) nearest distance to the wall


    //---------------------------------------//
    //                                       //
    //     to be assigned from world model   //
    //                                       //
    //---------------------------------------//

    int numb_rooms_detected = 0;    // total number of rooms detected
    int numb_rooms_entered = worldModel->get_enteredRooms();
    int numb_nexted_exits = worldModel->get_nestedExits();      // number of total exits in rooms - 1*numb_rooms_entered

    Location location = worldModel->get_currentLocation();
    bool object_found = false;      // second phase

    if (location.in_room){
        int current_room = 0;       // number of the room in which we are located now
        int numb_corners_detected = 0;
        int numb_exits = 0;
    }
    if (location.in_corridor){
        bool end_of_corridor = false;       // have PICO reached the end of the corridor once?
        bool at_start = false;              // have we returned back to the initial condition yet?
    }
    Point_det wall = worldModel->get_closestPointWall(); // has to be assigned as well


    //------------end of assigning variables-------------//




    switch(high_st) {
    case EXPLORE_HOSPITAL:

        std::cout << "EXPLORING THE HOSPITAL " << std::endl;


        if (location.in_corridor) {

            if (numb_rooms_detected == 0 || !end_of_corridor){
                low_st = EXPLORE_CORRIDOR;
                std::cout << "Exploring the corridor " << std::endl;
                break;
            }
            else if (numb_rooms_detected < numb_rooms_entered - numb_nexted_exits){
                low_st = EXIT_CORRIDOR;
                std::cout << "Exiting corridor " << std::endl;

                // location of exits must be checked in execution to prevent
                //                          entering the room we've been to
            }

            // command to go to  first exit
        } else if (location.in_room || location.in_nested_room) {

            switch(low_st){

            case EXPLORE_ROOM:

                std::cout << "Exploring the room " << std::endl;

                // leave the boolean true if we have been to the nested room
                // if we have been to the nested room and we are in a room (main room) now...
                if (location.in_room && location.in_nested_room){
                    low_st = EXIT_MAIN_ROOM;
                    std::cout << "Command to not to explore the room active " << std::endl;
                }
                else {
                    // check if all the corners are detected
                    if (numb_corners_detected == 4){

                        // if nested exit detected
                        if (numb_exits > 1) {
                            low_st = GO_TO_NEXT_ROOM;
                            std::cout << "Going to the nested room " << std::endl;
                        }
                        else {
                            low_st = EXIT;
                            std::cout << "Exiting to the corridor " << std::endl;
                        }
                    }
                }

                break;

            case EXIT:
                std::cout << "Moving towards exit " << std::endl;
                break;

            case GO_TO_NEXT_ROOM:
                std::cout << "Going to nested room " << std::endl;
                if (location.in_nested_room){
                    low_st = EXPLORE_ROOM;
                }
                break;

            }
        }


        if (numb_rooms_entered == (numb_nexted_exits+numb_rooms_entered)) {
            std::cout << "Moving to starting point " << std::endl;
            high_st = RETURN_TO_INIT;
        }

        break;

    case RETURN_TO_INIT:
        // assuming we are in the corridor
        std::cout << "RETURNING TO INITIAL POSITION " << std::endl;

        if (at_start){
            if (!parked_backwards){
                low_st = PARKING;
                std::cout << "Parking " << std::endl;
            } else {
                high_st = GO_TO_ROOM;
                std::cout << "Parked! " << std::endl;
                low_st = EXIT_CORRIDOR;
            }
        } else {
            low_st = GO_TO_START;
            std::cout << "Moving to starting point " << std::endl;
        }

        break;

    case GO_TO_ROOM:
        std::cout << "FINDING OBJECT PHASE " << std::endl;
        switch(low_st) {

        case EXIT_CORRIDOR:
            if (location.in_corridor){
                std::cout << "Exiting corridor " << std::endl;
            } else if (location.in_room){
                low_st = GO_TO_NEXT_ROOM;
            } else{
                std::cout << "ERROR exiting corridor " << std::endl;
            }
            break;

        case GO_TO_NEXT_ROOM:
            std::cout << "Going to nested room " << std::endl;
            if (object_found){
                low_st = STAND_NEXT_TO_OBJECT;
            }
            break;

        case STAND_NEXT_TO_OBJECT:
            std::cout << "Moving to the object " << std::endl;
            break;
        }

        break;
    }

}
