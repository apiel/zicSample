#ifndef _VIEW_H_
#define _VIEW_H_

#include "def.h"

class Grid {
public:
    int8_t row = 0;
    int8_t col = 0;

    uint8_t rows = 0;
    uint8_t cols = 0;

    Grid(uint8_t rows, uint8_t cols)
    {
        this->rows = rows;
        this->cols = cols;
    }

    void selectNextRow(int8_t dir)
    {
        row += dir;
        if (row >= rows) {
            row = rows - 1;
        } else if (row < 0) {
            row = 0;
        }

        printf("row %d\n", row);
    }

    void selectNextCol(int8_t dir)
    {
        col += dir;
        if (col >= cols) {
            col = cols - 1;
        } else if (col < 0) {
            col = 0;
        }

        printf("col %d\n", col);
    }
};

class View {
public:
    virtual void render() = 0;
    virtual uint8_t update(UiKeys& keys) = 0;
};

#endif
