#include "input.h"
#include <ncurses.h>

InputType Input::getInput()
{
    InputType res = InputType::Command;
    timeout(m_time);
    int key = getch();
    switch (key) {
    case KEY_UP:
        res = InputType::Move;
        m_move.x = 0;
        m_move.y = -1;
        break;
    case KEY_DOWN:
        res = InputType::Move;
        m_move.x = 0;
        m_move.y = 1;
        break;
    case KEY_LEFT:
        res = InputType::Move;
        m_move.x = -1;
        m_move.y = 0;
        break;
    case KEY_RIGHT:
        res = InputType::Move;
        m_move.x = 1;
        m_move.y = 0;
        break;
    }
    return res;
}
