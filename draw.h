#ifndef _DRAW_H_
#define _DRAW_H_

#include "def.h"
#include "state.h"
#include "drawButton.h"

void draw()
{
    SDL_RenderPresent(renderer);
}

int drawText(Point position, const char* text, SDL_Color color = APP_DEFAULT_FONT_COLOR, uint32_t size = APP_DEFAULT_FONT_SIZE, const char* fontPath = APP_FONT)
{
    TTF_Font* font = TTF_OpenFont(fontPath, size);
    if (font == NULL) {
        APP_LOG("Failed to open font\n");
        return 0;
    }
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) {
        APP_LOG("Failed to render text\n");
        return 0;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        APP_LOG("Failed to create texture\n");
        return 0;
    }

    SDL_Rect rect = { position.x, position.y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    int x = surface->w + position.x;

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);

    return x;
}

void drawClear(SDL_Color color = COLOR_BACKGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void drawFilledRect(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { position.x, position.y, size.w, size.h };
    SDL_RenderFillRect(renderer, &rect);
}

void drawRect(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { position.x, position.y, size.w, size.h };
    SDL_RenderDrawRect(renderer, &rect);
}

int drawLabelValue(Point position, const char* label, const char* value, const char* unit, bool selected = false, uint8_t size = 14)
{
    unsigned int x = position.x;
    if (label) {
        x = drawText({ x, position.y + 3 }, label, COLOR_LABEL, size - 3) + 2;
    }
    x = drawText({ x, position.y }, value, COLOR_INFO, size);
    if (unit) {
        x = drawText({ x + 1, position.y + 4 }, unit, COLOR_LABEL, size - 5);
    }
    if (selected) {
        drawRect({ position.x - 2, position.y }, { (x - position.x) + 4, 18 }, COLOR_INFO);
    }
    return x;
}

int drawLabelValue(Point position, const char* label, float value, const char* unit, bool selected = false, uint8_t size = 14)
{
    char str[16];
    sprintf(str, "%.1f", value);
    return drawLabelValue(position, label, str, unit, selected, size);
}

int drawLabelValue(Point position, const char* label, int value, const char* unit, bool selected = false, uint8_t size = 14)
{
    char str[16];
    sprintf(str, "%d", value);
    return drawLabelValue(position, label, str, unit, selected, size);
}

int drawSelectableText(bool selected, Point position, const char* text, SDL_Color color = APP_DEFAULT_FONT_COLOR, uint32_t size = APP_DEFAULT_FONT_SIZE, const char* fontPath = APP_FONT)
{
    int x = drawText(position, text, color, size, fontPath);
    if (selected) {
        drawRect({ position.x - 2, position.y }, { (x - position.x) + 4, size + 2 }, COLOR_INFO);
    }
    return x;
}

#endif