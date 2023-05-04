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
    };

    const uint32_t width = (SCREEN_W - 97) * 0.25;
    const uint32_t height = 60;
    const uint32_t halfHeight = height * 0.5 - 1;

    Btn btnX = { COLOR_BTN_X, "X", { 92, 5 }, { width, height } };
    Btn btnY = { COLOR_BTN_Y, "Y", { btnX.position.x + width + 2, btnX.position.y }, { width * 2, halfHeight } };
    Btn btnB = { COLOR_BTN_B, "B", { btnY.position.x, btnX.position.y + halfHeight + 2 }, { width * 2, halfHeight } };
    Btn btnA = { COLOR_BTN_A, "A", { 2 + btnY.position.x + width * 2, btnX.position.y }, { width, height } };

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

    void drawBtn(Btn& btn)
    {
        drawFilledRect(btn.position, btn.size, COLOR_FOREGROUND2);
        drawTriangle({ btn.position.x + btn.size.w, btn.position.y }, btn);
    }

public:
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
        drawFilledRect(btnX.position, { width * 4, height }, COLOR_BACKGROUND);

        drawY();
        drawX();
        drawB();
        drawA();
    }
};

#endif
