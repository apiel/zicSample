#ifndef _DRAW_H_
#define _DRAW_H_

#include "def.h"
#include "state.h"

void draw()
{
    SDL_RenderPresent(renderer);
}

void drawText(Point position, const char* text, SDL_Color color = APP_DEFAULT_FONT_COLOR, uint32_t size = APP_DEFAULT_FONT_SIZE, const char* fontPath = APP_FONT)
{
    TTF_Font* font = TTF_OpenFont(fontPath, size);
    if (font == NULL) {
        APP_LOG("Failed to open font\n");
        return;
    }
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) {
        APP_LOG("Failed to render text\n");
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        APP_LOG("Failed to create texture\n");
        return;
    }

    SDL_Rect rect = { (int)position.x, (int)position.y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // sizeObj.Set("w", surface->w);
    // sizeObj.Set("h", surface->h);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void drawClear(SDL_Color color = COLOR_BACKGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void drawFilledRect(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { (int)position.x, (int)position.y, (int)size.w, (int)size.h };
    SDL_RenderFillRect(renderer, &rect);
}

void drawRect(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { (int)position.x, (int)position.y, (int)size.w, (int)size.h };
    SDL_RenderDrawRect(renderer, &rect);
}

#endif