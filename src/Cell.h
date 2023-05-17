//
// Created by mcumbrella on 23-5-10.
//

#ifndef GOL_CELL_H
#define GOL_CELL_H

#include "CellState.h"
#include <string>

/**
 * 细胞是培养皿中的最小单元。
 * 除了作为边界的细胞，每个细胞都可以是活细胞或死细胞，
 * 具体情况取决于周边8个细胞的状态
 */
class Cell
{
private:
    CellState state = STATE_BORDER;
    CellState nextState = STATE_BORDER;
    Cell* neighbours[8] = {}; // 该细胞的内部缓存，存储指向周边8个细胞的指针
public:
    Cell() = default;

    explicit Cell(const CellState& state); // 初始化一个指定状态的细胞

    CellState getState() const; // 获取细胞的当前状态

    void setState(const CellState& newState); // 设置细胞当前的状态

    CellState getNextState() const; // 获取细胞下次迭代时的状态

    void setNextState(const CellState& newState); // 设置细胞下次迭代时的状态

    /**
     * 将细胞转换为字符串
     * @return 活细胞=方形，死细胞=点，
     * 具体外观取决于目标操作系统是否是Windows
     */
    std::string toString() const;

    /**
     * 将细胞转换为char
     * @return 活细胞='1'，死细胞='0'，边界细胞='#'
     */
    char toChar() const;

    /**
     * 根据该细胞周围8个细胞的状态计算下次迭代时的状态
     * @return
     * STATE_BORDER 如果:
     * <ul>
     * <li> 该细胞当前的状态是 STATE_BORDER </li>
     * </ul>
     * STATE_ALIVE 如果:
     * <ul>
     * <li> 该细胞周围有3个活细胞，或者 </li>
     * <li> 该细胞是活细胞，而且周围有2个活细胞 </li>
     * </ul>
     * STATE_DEAD 如果:
     * <ul>
     * <li> 上述条件都不满足 </li>
     * </ul>
     */
    CellState calculateNextState() const;

    /**
     * 依次设置指向该细胞周边8个细胞的指针。
     * 这个函数必须一次性对同一个细胞调用8次，
     * 否则后续对其它细胞的操作都会出错
     * @param c 一个指向相邻细胞的指针
     */
    Cell& setNeighbour(Cell* c);
};

#endif //GOL_CELL_H
