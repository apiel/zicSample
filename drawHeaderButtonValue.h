#ifndef _DRAW_HEADER_BUTTON_VALUE_H_
#define _DRAW_HEADER_BUTTON_VALUE_H_

#include "draw.h"

#include <vector>

class HeaderButtonValue {
protected:
    struct Btn {
        SDL_Color color;
        const char* letter;
        const Point position;
        const Size size;
        const char* label1 = NULL;
        const char* label2 = NULL;
    };

    const Size sizeVertical = { (SCREEN_W - 97) * 0.25, 60 };
    const Size sizeHorizontal = { sizeVertical.w * 2, sizeVertical.h * 0.5 - 1 };

    void drawTriangle(Point position, Btn& btn)
    {
        uint32_t size = 15;
        const std::vector<SDL_Vertex> verts = {
            {
                SDL_FPoint { position.x - size, position.y },
                btn.color,
                SDL_FPoint { 0 },
            },
            {
                SDL_FPoint { position.x, position.y },
                btn.color,
                SDL_FPoint { 0 },
            },
            {
                SDL_FPoint { position.x, position.y + size },
                btn.color,
                SDL_FPoint { 0 },
            },
        };
        SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);

        drawText({ position.x - (uint32_t)(size * 0.5), position.y }, btn.letter, COLOR_WHITE, 8);
    }

    bool isVertical(Btn& btn)
    {
        return btn.size.w == sizeVertical.w;
    }

    void drawBtn(Btn& btn)
    {
        drawFilledRect(btn.position, btn.size, COLOR_FOREGROUND2);
        drawTriangle({ btn.position.x + btn.size.w, btn.position.y }, btn);

        if (btn.label1 != NULL) {
            drawText({ btn.position.x + 5, btn.position.y }, btn.label1, COLOR_LABEL, 10);
        }

        if (btn.label2 != NULL) {
            if (isVertical(btn)) {
                // btnB.position.y because y = btnY.position.y + sizeHorizontal.h + 2
                drawText({ btn.position.x + 5, btnB.position.y }, btn.label2, COLOR_LABEL, 10);
            } else {
                drawText({ btn.position.x + 5, btn.position.y + 10 }, btn.label2, COLOR_LABEL, 10);
            }
        }
    }

public:
    Btn btnY = { COLOR_BTN_Y, "Y", { 92, 5 }, sizeVertical };
    Btn btnX = { COLOR_BTN_X, "X", { btnY.position.x + sizeVertical.w + 2, btnY.position.y }, sizeHorizontal };
    Btn btnB = { COLOR_BTN_B, "B", { btnX.position.x, btnY.position.y + sizeHorizontal.h + 2 }, sizeHorizontal };
    Btn btnA = { COLOR_BTN_A, "A", { 2 + btnX.position.x + sizeHorizontal.w, btnY.position.y }, sizeVertical };

    void drawY()
    {
        drawBtn(btnX);
    }

    void drawX()
    {
        drawBtn(btnY);
    }

    void drawB()
    {
        drawBtn(btnB);
    }

    void drawA()
    {
        drawBtn(btnA);
    }

    void draw()
    {
        // FIXME remove this
        drawFilledRect(btnY.position, { sizeVertical.w * 4, sizeVertical.h }, COLOR_BACKGROUND);

        drawY();
        drawX();
        drawB();
        drawA();
    }
};

#endif
