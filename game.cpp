#include "game.h"
#include <time.h>
#include <chrono>
#include <thread>

void Game::InitGraphics()
{
    initscr();
    keypad(stdscr, true);
    noecho();
    curs_set(0);
}

void Game::DrawField()
{
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field[i].size(); ++j) {
            move(i, j);
            addch(field[i][j]);
        }
    }
    refresh();
}

void Game::SnakeMove()
{
    Point newHead;
    newHead.x = first.x + deltaMove.x;
    newHead.y = first.y + deltaMove.y;
    if (newHead.x >= 0 && newHead.x < field[0].size() && newHead.y >= 0 && newHead.y < field.size()) {
        field[first.y][first.x] = CellType::Tail;
        field[newHead.y][newHead.x] = CellType::Head;
        first = newHead;
        DrawPixel(newHead.x, newHead.y);
    }
}

void Game::Tick()
{
    time_t finish_time = clock() / (CLOCKS_PER_SEC / 1000) + m_speed;
    InputType inputType = input.getInput();
    time_t curr_time = clock() / (CLOCKS_PER_SEC / 1000);
    if (curr_time < finish_time)
        std::this_thread::sleep_for(std::chrono::milliseconds(finish_time - curr_time));
    if (inputType == InputType::Move) {
        Point delta = input.getMove();
        if ((deltaMove.x != -delta.x) && (deltaMove.y != -delta.y)) {
            deltaMove = delta;
        }
    }
    SnakeMove();
}

Game::Game(int width, int height) : m_width(width), m_height(height), m_speed(1000)
{
    InitGraphics();
    field.resize(m_height);
    for (auto &subv: field) {
        subv.resize(m_width);
    }
    first.x = m_width / 2;
    first.y = m_height / 2;
    last.x = first.x;
    last.y = first.y;
    deltaMove.x = 1;
    deltaMove.y = 0;
    field[first.y][first.x].setType(CellType::Head);
    input.setTime(m_speed);
    DrawField();
}


