//
// Created by mcumbrella on 23-5-10.
//

#ifndef GOL_COMMONUTIL_H
#define GOL_COMMONUTIL_H

#include "CellState.h"

class CommonUtil
{
public:
    static CellState parseCellState(const char& c);

    static void freeze(const unsigned int& ms);

    static void clearScreen();
};

#endif //GOL_COMMONUTIL_H
