#include "def.h"
#include "sdl2.h"
#include "view_main.h"

// UI_Display uiDisplay;
// App *app = App::getInstance(&uiDisplay);

void audioCallBack(void* userdata, Uint8* stream, int len)
{
    // float* buf = (float*)stream;
    // app->sample(buf, len);
}

int main(int argc, char* args[])
{
    SDL_Log(">>>>>>> Start Zic Tracker\n");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

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

    SDL_AudioDeviceID audioDevice = initAudio(audioCallBack);
    if (SDL_getenv("ZIC_SKIP_AUDIO") == NULL && !audioDevice) {
        return 1;
    }

    ViewMain::getInstance()->render();
    SDL_RenderPresent(renderer);

    uint8_t renderCount = 0;
    while (handleEvent()) {
        if (ui.keysChanged) {
            ui.keysChanged = false;
            // app->handleUi(ui.keys);
            // SDL_Log("\n%s\n", uiDisplay.text);
            // SDL_UpdateWindowSurface(window);
        }
        SDL_Delay(10);
        if (renderCount < 200) {
            renderCount++;
            SDL_RenderPresent(renderer);
        }
        // if (app->rendered) {
        //     app->rendered = false;
        //     SDL_UpdateWindowSurface(window);
        // }

        // SDL_Delay(10);
        // ViewMain::getInstance()->render();
        // SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audioDevice);

    SDL_Quit();
    return 0;
}