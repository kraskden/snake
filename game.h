#ifndef GAME_H
#define GAME_H

#include <vector>
#include <list>
#include <ncurses.h>

#include "cell.h"
#include "input.h"

using std::vector;
using std::list;

class Game
{
    int m_width, m_height;
    int m_speed, m_food_percentage;
    bool is_played;
    vector<vector<Cell>> field;
    list<Point> snake;
    Point deltaMove;
    Input input;
    void InitNcursed();
    void InitField();
    void DrawHeader();
    void DrawField();
    void SnakeMove();
    void SpawnFood();
    inline void DrawPixel(int x, int y) {
        move(y + 1, x + 1);
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
