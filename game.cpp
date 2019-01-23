#include "game.h"
#include <time.h>
#include <chrono>
#include <thread>

void Game::InitNcursed()
{
    initscr();
    keypad(stdscr, true);
    noecho();
    curs_set(0);
}

void Game::InitField()
{
    field.resize(m_height);
    for (auto &subv: field) {
        subv.resize(m_width);
        for (auto &el : subv)
            el = CellType::None;
    }
    snake.push_front(Point(m_width / 2, m_height / 2));
    deltaMove.x = 1;
    deltaMove.y = 0;
    field[m_height / 2][m_width / 2].setType(CellType::Head);
}

void Game::DrawHeader()
{
    addch('+');
    for (int i = 0; i < m_width; ++i)
        addch('-');
    addch('+');
    addch('\n');
}

void Game::DrawField()
{
    move(0, 0);
    DrawHeader();
    for (int i = 0; i < m_height; ++i) {
        addch('|');
        for (int j = 0; j < m_width; ++j) {
            addch(field[i][j]);
        }
        addch('|');
        addch('\n');
    }
    DrawHeader();
    refresh();
}

void Game::SnakeMove()
{
    bool isFood, isWall, isTeleport;
    Point &first = snake.front();
    Point &last = snake.back();
    Point newHead(first.x + deltaMove.x, first.y + deltaMove.y);
    isTeleport = !(newHead.x >= 0 && newHead.x < m_width && newHead.y >= 0 && newHead.y < m_height);
    if (!isTeleport) {
        CellType next = field[newHead.y][newHead.x].getType();
        isWall = (next == CellType::Wall) || (next == CellType::Tail);
        isFood = next == CellType::Food;
    }
    if (isWall) {
        is_played = false;
        return;
    }
    if (!isTeleport) {
        field[first.y][first.x] = CellType::Tail;
        field[newHead.y][newHead.x] = CellType::Head;
        if (!isFood)
            field[last.y][last.x] = CellType::None;
        DrawPixel(newHead.x, newHead.y);
        DrawPixel(first.x, first.y);
        DrawPixel(last.x, last.y);
        snake.push_front(newHead);
        if (!isFood)
            snake.pop_back();
    }
}

void Game::SpawnFood()
{
    int x, y;
    do {
        x = rand() % m_width;
        y = rand() % m_height;
    } while (field[y][x].getType() != CellType::None);
    field[y][x].setType(CellType::Food);
    DrawPixel(x, y);
}

void Game::Tick()
{
    time_t finish_time = clock() / (CLOCKS_PER_SEC / 1000) + m_speed;
    input.setTime(m_speed);
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
    if ((rand() % 100) <= m_food_percentage)
        SpawnFood();

}

Game::Game(int width, int height) : m_width(width), m_height(height), m_speed(400), m_food_percentage(20),
    is_played(true)
{
    srand(time(nullptr));
    InitNcursed();
    InitField();
    DrawField();
}
