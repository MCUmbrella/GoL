//
// Created by mcumbrella on 23-5-10.
//

#include "Cell.h"

std::string Cell::toString() const
{
#ifdef _WIN32
    return state == STATE_BORDER ? "##" :
           state == STATE_DEAD ? ". "
                               : "[]";
#else
    return state == STATE_BORDER ? "囗" :
           state == STATE_DEAD ? "丶"
                               : "回";
#endif
}

Cell::Cell(const CellState& state)
{
    this->state = state;
    this->nextState = state;
}

CellState Cell::getState() const
{
    return state;
}

void Cell::setState(const CellState& newState)
{
    state = newState;
}

CellState Cell::getNextState() const
{
    return nextState;
}

void Cell::setNextState(const CellState& newState)
{
    nextState = newState;
}

char Cell::toChar() const
{
    return state == STATE_BORDER ? '#' :
           state == STATE_DEAD ? '0' :
           '1';
}

CellState Cell::calculateNextState() const
{
    if (state == STATE_BORDER) return STATE_BORDER;
    int live = 0; // 周围活细胞数
    for (const auto& c : neighbours)
        if (c->getState() == STATE_ALIVE) ++live;
    return live == 3 || (state == STATE_ALIVE && live == 2) ? STATE_ALIVE : STATE_DEAD;
}

Cell& Cell::setNeighbour(Cell* c)
{
    static unsigned char i = 0; // static表示i对于所有细胞实例都具有同一值，方便复用
    i %= 8; // i为8即表示一个细胞的周边缓存设置完毕，重置为0以便下个细胞进行设置
    neighbours[i] = c;
    ++i;
    return *this;
}
