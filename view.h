#ifndef _VIEW_H_
#define _VIEW_H_

#include "def.h"

class Grid {
public:
    int8_t row = 0;
    int8_t col = 0;

    int8_t lastRow = 0;
    int8_t lastCol = 0;

    uint8_t rows = 0;
    uint8_t cols = 0;

    Grid(uint8_t rows, uint8_t cols)
    {
        this->rows = rows;
        this->cols = cols;
    }

    void selectNextRow(int8_t dir)
    {
        lastRow = row;
        lastCol = col;
        row += dir;
        if (row >= rows) {
            row = rows - 1;
        } else if (row < 0) {
            row = 0;
        }
    }

    void selectNextCol(int8_t dir)
    {
        lastRow = row;
        lastCol = col;
        col += dir;
        if (col >= cols) {
            col = cols - 1;
        } else if (col < 0) {
            col = 0;
        }
    }

    bool is(int8_t row, int8_t col)
    {
        return this->row == row && this->col == col;
    }

    uint8_t update(UiKeys& keys)
    {
        if (keys.Up) {
            selectNextRow(-1);
        } else if (keys.Down) {
            selectNextRow(+1);
        } else if (keys.Left) {
            selectNextCol(-1);
        } else if (keys.Right) {
            selectNextCol(+1);
        } else {
            return VIEW_NONE;
        }
        return VIEW_CHANGED;
    }
};

class View {
public:
    virtual void render() = 0;
    virtual void update(UiKeys& keys) = 0;
};

#endif
