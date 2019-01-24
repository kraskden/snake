#include "game.h"
#include <time.h>
#include <chrono>
#include <thread>

void DestroyMoney(Game* game)
{
    game->field[game->m_money_y][game->m_money_x] = CellType::None;
    game->DrawPixel(game->m_money_x, game->m_money_y);
}

void Game::InitNcursed()
{
    initscr();
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    start_color();
    init_pair(static_cast<short>(CellType::Head), COLOR_YELLOW, COLOR_BLACK);
    init_pair(static_cast<short>(CellType::Food), COLOR_CYAN, COLOR_BLACK);
    init_pair(static_cast<short>(CellType::Tail), COLOR_WHITE, COLOR_BLACK);
    init_pair(static_cast<short>(CellType::Wall), COLOR_GREEN, COLOR_BLACK);
    init_pair(static_cast<short>(CellType::Money), COLOR_RED, COLOR_BLACK);
    attrset(COLOR_PAIR(0));
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
        addch('+');
    addch('+');
    addch('\n');
}

void Game::DrawField()
{
    move(0, 0);
    DrawHeader();
    for (int i = 0; i < m_height; ++i) {
        addch('+');
        for (int j = 0; j < m_width; ++j) {
            short type = static_cast<short>(field[i][j].getType());
            attrset(COLOR_PAIR(type));
            addch(field[i][j]);
            attrset(COLOR_PAIR(0)); // To default
        }
        addch('+');
        addch('\n');
    }
    DrawHeader();
    refresh();
}

void Game::DrawScore()
{
    std::string print = std::to_string(m_score);
    move(m_score_coord.y, m_score_coord.x);
    addstr(print.c_str());
    refresh();
}

void Game::DrawBar()
{
    if (m_max_time == 0)
        return;
    move(m_time_bar_coord.y, m_time_bar_coord.x);
    for (int i = 0; i < (m_time * 10 / m_max_time); ++i)
        addch('|');
    for (int i = (m_time * 10 / m_max_time); i < 10; ++i)
        addch(' ');
}

void Game::SnakeMove()
{
    bool isFood, isWall, isTeleportX, isTeleportY;
    Point &first = snake.front();
    Point &last = snake.back();
    Point newHead(first.x + deltaMove.x, first.y + deltaMove.y);
    isTeleportX = !(newHead.x >= 0 && newHead.x < m_width);
    isTeleportY = !(newHead.y >= 0 && newHead.y < m_height);
    if (isTeleportX)
        newHead.x = (newHead.x == -1) ? m_width - 1 : 0;
    if (isTeleportY)
        newHead.y = (newHead.y == -1) ? m_height - 1 : 0;
    CellType next = field[newHead.y][newHead.x].getType();
    isWall = (next == CellType::Wall) || (next == CellType::Tail);
    isFood = next == CellType::Food;
    if (isWall) {
        is_played = false;
        return;
    }
    if (next == CellType::Money) {
        m_score += 5;
        DrawScore();
        m_time = 0;
        m_time_bar_call = nullptr;
        DrawBar();
    }
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
    else {
        ++m_score;
        DrawScore();
        SpawnItem(CellType::Food);
        if ((rand() % 100) < m_money_percentage && m_time == 0) {
            m_time = m_max_time = 15;
            DrawBar();
            m_time_bar_call = DestroyMoney;
            SpawnItem(CellType::Money);
        }
    }
}

void Game::SpawnItem(CellType item)
{
    int x, y;
    int try_spawn = 0;
    do {
        x = rand() % m_width;
        y = rand() % m_height;
    } while (field[y][x].getType() != CellType::None && (++try_spawn < 10));
    if (try_spawn == 10)
        return;
    field[y][x].setType(item);
    if (item == CellType::Money) {
        m_money_x = x;
        m_money_y = y;
    }
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
    } else if (inputType == InputType::Command) {
        switch (input.getCommand()) {
        case InputCommand::Quit:
            is_played = false;
            break;
        case InputCommand::Pause:
            while (input.getInput() != InputType::Command || input.getCommand() != InputCommand::Pause) {}
            break;
        case InputCommand::Resize:
            clear();
            DrawField();
            break;
        }
    }
    SnakeMove();
    if (m_time > 0) {
        DrawBar();
        if (--m_time == 0 && m_time_bar_call)
            m_time_bar_call(this);
    }
}

Game::Game(int width, int height) : m_width(width), m_height(height), m_speed(200), m_money_percentage(20),
    m_score(0), m_time_bar_call(nullptr), is_played(true)
{
    input.setTime(m_speed);
    srand(time(nullptr));
    InitNcursed();
    InitField();
    DrawField();
    m_score_coord.x = m_width + 2 + 3;
    m_score_coord.y = 2;
    m_time_bar_coord.x = m_score_coord.x;
    m_time_bar_coord.y = 3;
    DrawScore();
    SpawnItem(CellType::Food);
}
