#include "audioHandler.h"
#include "data.h"
#include "def.h"
#include "sdl2.h"
#include "viewMain.h"

AudioHandler& audio = AudioHandler::get();

void audioCallBack(void* userdata, Uint8* stream, int len)
{
    float* buf = (float*)stream;
    audio.sample(buf, len);
}

int main(int argc, char* args[])
{
    SDL_Log(">>>>>>> Start Zic Tracker\n");

    Data::get().load();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Log("SDL video driver: %s\n", SDL_GetCurrentVideoDriver());
    SDL_Log("SDL audio driver: %s\n", SDL_GetCurrentAudioDriver());

    SDL_Window* window = SDL_CreateWindow(
        "Zic",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    TTF_Init();

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_AudioDeviceID audioDevice = initAudio(audioCallBack);
    if (SDL_getenv("ZIC_SKIP_AUDIO") == NULL && !audioDevice) {
        return 1;
    }

    ViewMain& viewMain = ViewMain::get();
    viewMain.render();
    SDL_RenderPresent(renderer);

    uint8_t renderCount = 0;
    while (handleEvent()) {
        bool rendered = false;
        if (ui.keysChanged) {
            ui.keysChanged = false;
            UiKeys keys(ui.keys);
            rendered = viewMain.update(keys);
        }
        SDL_Delay(10);
        // ensure rendering on RG351P
        if (renderCount < 200) {
            renderCount++;
            SDL_RenderPresent(renderer);
        }
        if (rendered) {
            viewMain.render();
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audioDevice);

    SDL_Quit();
    return 0;
}