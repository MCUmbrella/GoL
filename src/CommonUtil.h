//
// Created by mcumbrella on 23-5-10.
//

#ifndef GOL_COMMONUTIL_H
#define GOL_COMMONUTIL_H

#include "CellState.h"

/**
 * 工具类，包含各种有用的函数
 */
class CommonUtil
{
public:
    /**
     * 将char解析为CellState
     * @return '1'为STATE_ALIVE，'0'为STATE_DEAD，其它为STATE_BORDER
     */
    static CellState parseCellState(const char& c);

    /**
     * 暂停程序一段时间
     * @param ms 暂停多少毫秒？
     */
    static void freeze(const unsigned int& ms);

    /**
     * 清除控制台上显示的内容
     */
    static void clearScreen();

    /**
     * 用于映射透明边界下的坐标
     * @code
     *            / l               (x mod l = 0)
     * t(x, l) = {  x mod l         (x ＞ 0)
     *            \ l - (-x mod l)  (x ＜ 0)
     */
    static int transparent(const int& x, const int& l);
};

#endif //GOL_COMMONUTIL_H
