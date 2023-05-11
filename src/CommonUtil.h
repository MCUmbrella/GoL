//
// Created by mcumbrella on 23-5-10.
//

#ifndef GOL_COMMONUTIL_H
#define GOL_COMMONUTIL_H

#include "CellState.h"

class CommonUtil
{
public:
    /**
     * Parses the CellState from a character.
     * @param c The character to parse.
     * @return STATE_ALIVE if '1', STATE_DEAD if '0', otherwise STATE_BORDER.
     */
    static CellState parseCellState(const char& c);

    /**
     * Freezes the program for a few moment.
     * @param ms The milliseconds of the time to freeze.
     */
    static void freeze(const unsigned int& ms);

    /**
     * Clears the terminal emulator.
     */
    static void clearScreen();

    /**
     * @code
     *            / l               (x mod l = 0)
     * t(x, l) = {  x mod l         (x ＞ 0)
     *            \ l - (-x mod l)  (x ＜ 0)
     */
    static int transparent(const int& x, const int& l);
};

#endif //GOL_COMMONUTIL_H
