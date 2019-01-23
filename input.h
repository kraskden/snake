#ifndef INPUT_H
#define INPUT_H

#include "cell.h"

enum class InputType {
    Move, Command, Nop
};

enum class InputCommand {
    Pause, Quit, Resize
};

class Input {
    InputType m_type;
    InputCommand m_cmd;
    Point m_move;
    int m_time;
public:
    Input(int time = 1000) : m_time(time) {}
    InputType getInput();
    InputType getType() {return m_type; }
    InputCommand getCommand() {return m_cmd; }
    Point getMove() {return m_move;}
    void setTime(int time) {m_time = time; }
};

#endif // INPUT_H
