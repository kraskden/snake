#ifndef GAME_H
#define GAME_H

#include <vector>
#include <ncurses.h>

#include "cell.h"
#include "input.h"

using std::vector;


class Game
{
    int m_width, m_height;
    int m_speed;
    bool is_played;
    vector<vector<Cell>> field;
    Point first, last;
    Point deltaMove;
    Input input;
    void InitGraphics();
    void DrawField();
    void SnakeMove();
    inline void DrawPixel(int x, int y) {
        move(y, x);
        addch(field[y][x]);
        refresh();
    }
public:
    Game(int width = 10, int height = 20);
    ~Game() {
        endwin();
    }
    bool isPlayed() {return is_played; }
    void Tick();

};

#endif // GAME_H
