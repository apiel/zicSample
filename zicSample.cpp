#include "sdl2.h"
#include "app_def.h"

// UI_Display uiDisplay;
// App *app = App::getInstance(&uiDisplay);

void audioCallBack(void* userdata, Uint8* stream, int len)
{
    float* buf = (float*)stream;
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
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
    // uiDisplay.init(screenSurface);

    SDL_AudioDeviceID audioDevice = initAudio(audioCallBack);
    if (SDL_getenv("ZIC_SKIP_AUDIO") == NULL && !audioDevice) {
        return 1;
    }

    // app->start();
    // app->render();
    SDL_UpdateWindowSurface(window);

    while (handleEvent()) {
        if (ui.keysChanged) {
            ui.keysChanged = false;
            // app->handleUi(ui.keys);
            // SDL_Log("\n%s\n", uiDisplay.text);
            // SDL_UpdateWindowSurface(window);
        }
        // SDL_Delay(10);
        // if (app->rendered) {
        //     app->rendered = false;
        //     SDL_UpdateWindowSurface(window);
        // }
    }
    // app->quit();

    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audioDevice);

    SDL_Quit();
    return 0;
}