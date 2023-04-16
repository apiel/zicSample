#ifndef _DRAW_H_
#define _DRAW_H_

#include "def.h"
#include "state.h"

void drawText(Point position, const char *text, Color color = APP_DEFAULT_FONT_COLOR, uint32_t size = APP_DEFAULT_FONT_SIZE)
{
    TTF_Font* font = TTF_OpenFont(APP_DEFAULT_FONT, size);
    if (font == NULL) {
        APP_LOG("Failed to open font\n");
        return;
    }
    SDL_Color sdlColor = { (uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, (uint8_t)color.a };
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, sdlColor);
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

#endif