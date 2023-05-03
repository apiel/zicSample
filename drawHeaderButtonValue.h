#ifndef _DRAW_HEADER_BUTTON_VALUE_H_
#define _DRAW_HEADER_BUTTON_VALUE_H_

#include "draw.h"

class HeaderButtonValue {
protected:
    const uint32_t width = (SCREEN_W - 97) * 0.25;
    const uint32_t height = 60;
    const uint32_t halfHeight = height * 0.5 - 1;
    const uint32_t Yx = 92;
    const uint32_t Yy = 5;
    const uint32_t Xx = Yx + width + 2;
    const uint32_t Xy = Yy;
    const uint32_t Bx = Xx;
    const uint32_t By = Yy + halfHeight + 2;
    const uint32_t Ax = 2 + Xx + width * 2;
    const uint32_t Ay = Yy;

public:
    void draw()
    {
        drawFilledRect({ Yx, Yy }, { width * 4, height }, COLOR_BACKGROUND);

        // drawFilledRect({ Yx, Yy }, { width, height }, COLOR_BTN_Y_ALPHA);
        // drawFilledRect({ Xx, Xy }, { width * 2, halfHeight }, COLOR_BTN_X_ALPHA);
        // drawFilledRect({ Bx, By }, { width * 2, halfHeight }, COLOR_BTN_B_ALPHA);
        // drawFilledRect({ Ax, Ay }, { width, height }, COLOR_BTN_A_ALPHA);

        drawFilledRect({ Yx, Yy }, { width, height }, COLOR_FOREGROUND2);
        drawFilledRect({ Xx, Xy }, { width * 2, halfHeight }, COLOR_FOREGROUND2);
        drawFilledRect({ Bx, By }, { width * 2, halfHeight }, COLOR_FOREGROUND2);
        drawFilledRect({ Ax, Ay }, { width, height }, COLOR_FOREGROUND2);

        // drawRect({ Yx, Yy }, { width, height }, COLOR_BTN_Y);
        // drawRect({ Xx, Xy }, { width * 2, halfHeight }, COLOR_BTN_X);
        // drawRect({ Bx, By }, { width * 2, halfHeight }, COLOR_BTN_B);
        // drawRect({ Ax, Ay }, { width, height }, COLOR_BTN_A);
    }
};

#endif
