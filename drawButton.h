#ifndef _DRAW_BUTTON_H_
#define _DRAW_BUTTON_H_

#include "def.h"
#include "state.h"

void drawPlayButton(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Point points[4] = {
        { position.x, position.y },
        { position.x + size.w, position.y + size.h * 0.5 },
        { position.x, position.y + size.h },
        { position.x, position.y }
    };
    SDL_RenderDrawLines(renderer, points, 4);
}

void drawArrowDown(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Point points[2] = {
        { position.x + size.w * 0.5, position.y },
        { position.x + size.w * 0.5, position.y + size.h * 0.9 }
    };
    SDL_RenderDrawLines(renderer, points, 2);

    SDL_Point points2[3] = {
        { position.x + size.w * 0.1, position.y + size.h * 0.5 },
        { position.x + size.w * 0.5, position.y + size.h * 0.9 },
        { position.x + size.w * 0.9, position.y + size.h * 0.5 }
    };
    SDL_RenderDrawLines(renderer, points2, 3);
}

void drawDownloadButton(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    drawArrowDown({ position.x + 2, position.y + 2 }, { size.w - 4, size.h - 4 }, color);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Point points[4] = {
        { position.x, position.y + size.h * 0.8 },
        { position.x, position.y + size.h },
        { position.x + size.w, position.y + size.h },
        { position.x + size.w, position.y + size.h * 0.8 }
    };
    SDL_RenderDrawLines(renderer, points, 4);
}

void drawNextButton(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Point points[3] = {
        { position.x, position.y },
        { position.x + size.w, position.y + size.h * 0.5 },
        { position.x, position.y + size.h }
    };
    SDL_RenderDrawLines(renderer, points, 3);
}

void drawPreviousButton(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Point points[3] = {
        { position.x + size.w, position.y },
        { position.x, position.y + size.h * 0.5 },
        { position.x + size.w, position.y + size.h }
    };
    SDL_RenderDrawLines(renderer, points, 3);
}

// void drawExitButton(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
// {
//     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

//     SDL_Point points2[2] = {
//         { position.x, position.y },
//         { position.x + size.w, position.y + size.h }
//     };
//     SDL_RenderDrawLines(renderer, points2, 2);

//     SDL_Point points3[2] = {
//         { position.x + size.w, position.y },
//         { position.x, position.y + size.h }
//     };
//     SDL_RenderDrawLines(renderer, points3, 2);
// }

// void drawSearchButton(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
// {
//     float radius = 0.65;
//     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
//     SDL_Point points[5] = {
//         { position.x, position.y },
//         { position.x + size.w * radius, position.y },
//         { position.x + size.w * radius, position.y + size.h * radius },
//         { position.x, position.y + size.h * radius },
//         { position.x, position.y }
//     };
//     SDL_RenderDrawLines(renderer, points, 5);

//     SDL_Point points2[2] = {
//         { position.x + size.w * radius, position.y + size.h * radius },
//         { position.x + size.w, position.y + size.h }
//     };
//     SDL_RenderDrawLines(renderer, points2, 2);
// }

#endif