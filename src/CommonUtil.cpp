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
    Sleep(ms);
#else
    usleep(ms * 1000U);
#endif
}

void CommonUtil::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J" << "\033[H";
#endif
}
