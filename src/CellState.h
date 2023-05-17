//
// Created by mcumbrella on 23-5-10.
//

#ifndef GOL_CELLSTATE_H
#define GOL_CELLSTATE_H

/**
 * 定义细胞的状态
 */
enum CellState
{
    STATE_BORDER = -1, // 边界
    STATE_DEAD = 0, // 死细胞
    STATE_ALIVE = 1 // 活细胞
};

#endif //GOL_CELLSTATE_H
