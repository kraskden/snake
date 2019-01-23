#ifndef CELL_H
#define CELL_H

struct Point {
    int x;
    int y;
    Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
};

enum class CellType {
    None, Head, Tail, Wall, Food, Money
};

class Cell {
    CellType m_type;
public:
    Cell(CellType type = CellType::None) : m_type(type) {}
    CellType getType() {return m_type;}
    void setType(CellType type) {m_type = type; }
    operator char();
};

#endif // CELL_H
