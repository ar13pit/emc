#ifndef MAIN_HPP
#define MAIN_HPP


typedef struct {
    bool in_process;    // should be active when turning 180
    bool in_corridor;   // check the phase of the escape
    bool turned_once;   // in the room when turned once
} Flags;

#endif
