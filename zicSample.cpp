#include "audioHandler.h"
#include "data.h"
#include "def.h"
#include "sdl2.h"
#include "viewMain.h"
#include "popupMessage.h"

#ifdef FREESOUND_ENABLED
#include "viewFreesound.h"
#endif

AudioHandler& audio = AudioHandler::get();

float lenDivider = 1.0f / sizeof(float);
void audioCallBack(void* userdata, Uint8* stream, int len)
{
    float* buf = (float*)stream;
    audio.samples(buf, len * lenDivider);
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

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
    SDL_SetRenderTarget(renderer, texture);

    SDL_AudioDeviceID audioDevice = initAudio(audioCallBack);
    if (SDL_getenv("ZIC_SKIP_AUDIO") == NULL && !audioDevice) {
        return 1;
    }

    initGameController();

    ViewMain& viewMain = ViewMain::get();
    viewMain.render();

    // ensure rendering on RG351P
    SDL_Delay(100);
    draw();

    PopupMessage& popupMessage = PopupMessage::get();
    unsigned long lastUpdate = SDL_GetTicks();
    while (handleEvent()) {
        unsigned long now = SDL_GetTicks();
        if (ui.keys.update || (ui.keys.controllerDirectional && now - lastUpdate > 150)) {
            ui.keys.update = false;
            switch (ui.view) {
#ifdef FREESOUND_ENABLED
            case VIEW_FREESOUND:
                ViewFreesound::get().handle(ui.keys);
                break;
#endif
            default:
                viewMain.handle(ui.keys);
                break;
            }
            lastUpdate = now;
        }
        if (ui.needMainViewRender) {
            ui.needMainViewRender = false;
            viewMain.render();
            draw();
        }
        if (needToRenderProgressBar && ui.view == VIEW_MAIN) {
            needToRenderProgressBar = false;
            ProgressBar::get().render(audio.stepCounter);
            draw();
        }
        popupMessage.handleRendering(now);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audioDevice);

    SDL_Quit();
    return 0;
}