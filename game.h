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
    int m_speed, m_money_percentage;
    int m_score;
    int m_time;
    int m_max_time;
    void (*m_time_bar_call)(Game*);
    int m_money_x, m_money_y;
    bool is_played;
    Point m_score_coord;
    Point m_time_bar_coord;
    vector<vector<Cell>> field;
    list<Point> snake;
    Point deltaMove;
    Input input;
    friend void DestroyMoney(Game* game);
    void InitNcursed();
    void InitField();
    void DrawHeader();
    void DrawField();
    void DrawScore();
    void DrawBar();
    void SnakeMove();
    void SpawnItem(CellType item);
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
