#ifndef MAIN_HPP
#define MAIN_HPP


typedef struct {
    bool in_corridor;   // check the phase of the escape
    bool turned_once;   // in the room when turned once
    bool escaped;       // finished
    bool drive_frw;
    bool turn;          // should be active when turning 180
} Flags;

#endif
