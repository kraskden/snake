#include "cell.h"

Cell::operator char()
{
    switch (m_type) {
    case CellType::None:
        return ' ';
    case CellType::Head:
        return '*';
    case CellType::Tail:
        return '*';
    case CellType::Wall:
        return '#';
    case CellType::Food:
        return '@';
    }
}
