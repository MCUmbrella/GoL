//
// Created by mcumbrella on 23-5-10.
//

#include "CommonUtil.h"
#include <iostream>

#ifdef _WIN32

#include <windows.h>

#else

#include <unistd.h>

#endif

CellState CommonUtil::parseCellState(const char& c)
{
    return c == '0' ? STATE_DEAD :
           c == '1' ? STATE_ALIVE :
           STATE_BORDER;
}

void CommonUtil::freeze(const unsigned int& ms)
{
#ifdef _WIN32
    Sleep(ms); // 使用Windows自带的Sleep函数
#else
    usleep(ms * 1000U); // 目标系统不是Windows，使用UNIX标准定义的usleep函数
#endif
}

void CommonUtil::clearScreen()
{
#ifdef _WIN32
    system("cls"); // 使用Windows自带的cls命令（极慢，可能导致画面撕裂）
#else
    std::cout << "\033[2J" << "\033[H"; // 目标系统不是Windows，使用VT100控制字符
#endif
}

int CommonUtil::transparent(const int& x, const int& l)
{
    return x % l == 0 ? l :
           x > 0 ? x % l :
           l - (-x % l);
}
