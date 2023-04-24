#ifndef _APP_SDL2_H_
#define _APP_SDL2_H_

#include "def.h"
#include "state.h"

bool handleKeyboard(SDL_KeyboardEvent* event)
{
    bool isDown = event->type == SDL_KEYDOWN;

    // we could skip keyChange on A repeat
    ui.keysChanged = true;

    // SDL_Log("handleKeyboard %d\n", event->keysym.scancode);
    // SDL_Log("handleKeyboard %d\n", event->repeat);
    switch (event->keysym.scancode) {
    case SDL_SCANCODE_UP:
        ui.keys.Up = isDown;
        return true;
    case SDL_SCANCODE_DOWN:
        ui.keys.Down = isDown;
        return true;
    case SDL_SCANCODE_LEFT:
        ui.keys.Left = isDown;
        return true;
    case SDL_SCANCODE_RIGHT:
        ui.keys.Right = isDown;
        return true;
    case SDL_SCANCODE_S:
        ui.keys.Edit = isDown;
        return true;
    case SDL_SCANCODE_Z: // Y
        ui.keys.Edit2 = isDown;
        return true;
    case SDL_SCANCODE_A:
        ui.keys.Action = isDown;
        return true;
    case SDL_SCANCODE_Q:
        ui.keys.Mode = isDown;
        return true;
    case SDL_SCANCODE_SPACE:
        ui.keys.Menu = isDown;
        return true;
    case SDL_SCANCODE_ESCAPE:
        return false;
    default:
        return true;
    }

    return true;
}

bool handleController(SDL_ControllerButtonEvent* event)
{
    bool isDown = event->state == SDL_PRESSED;

    // we could skip keyChange on A repeat
    ui.keysChanged = true;

    // SDL_Log("handleController btn %d state %d\n", event->button, event->state);
    SDL_Log("handleController btn %d state %d type %d which %d pad1 %d pad2 %d\n",
        event->button, event->state, event->type, event->which, event->padding1, event->padding2);
    switch (event->button) {
    case 11:
        ui.keys.Up = isDown;
        return true;
    case 12:
        ui.keys.Down = isDown;
        return true;
    case 13:
        ui.keys.Left = isDown;
        return true;
    case 14:
        ui.keys.Right = isDown;
        return true;
    case 0: // A
        ui.keys.Edit = isDown;
        return true;
    case 1: // B
        ui.keys.Action = isDown;
        return true;
    case 3: // Y
        ui.keys.Edit2 = isDown;
        return true;
    case 2: // L1
        return true;
    case 5: // R1 ?
        return true;
    case 4: // L2
        return true;
    case 6: // R2
        return true;
    case 9: // start
        ui.keys.Menu = isDown;
        return true;
    case 10: // select
        ui.keys.Mode = isDown;
        return true;
    default:
        return true;
    }

    return true;
}

bool handleAxisEvent(SDL_JoyAxisEvent* event)
{
    // SDL_Log("handleAxisEvent axis %d value %d type %d which %d pad1 %d pad2 %d\n",
    //     event->axis, event->value, event->type, event->which, event->padding1, event->padding2);

    // float value = roundf(event->value / 32768.0f * 100.0f) * 0.01f; // roundf(val * 100) / 100
    float value = roundf(event->value * 0.00305175f) * 0.01f;
    switch (event->axis) {
    case 0: // left/right (left joystick)
        SDL_Log("handleAxisEvent axis X1 value %f\n", value);
        return true;
    case 1: // up/down (left joystick)
        SDL_Log("handleAxisEvent axis Y1 value %f\n", value);
        return true;
    case 2: // left/right (right joystick)
        SDL_Log("handleAxisEvent axis X2 value %f\n", value);
        return true;
    case 3: // up/down (right joystick)
        SDL_Log("handleAxisEvent axis Y2 value %f\n", value);
        return true;
    default:
        return true;
    }

    return true;
}

void initGameController()
{
    int num = SDL_NumJoysticks();
    SDL_Log("%d joysticks connected\n", num);
    for (int i = 0; i < num; i++) {
        SDL_JoystickOpen(i);
        SDL_Joystick* joystick = SDL_JoystickOpen(i);
        SDL_Log("Joystick %d: %s\n", i, SDL_JoystickName(joystick));
        if (SDL_IsGameController(i)) {
            SDL_GameControllerOpen(i);
            SDL_GameController* controller = SDL_GameControllerOpen(i);
            SDL_Log("Game controller %d: %s\n", i, SDL_GameControllerName(controller));
        }
    }
}

bool handleEvent()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        // if (event.type > 0x300 && event.type < 0x800) {
        //     SDL_Log("handleEvent %d\n", event.type);
        // }

        switch (event.type) {
        case SDL_QUIT:
            return false;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
            return handleKeyboard(&event.key);
            // could be useful to simulate a pot
            // case SDL_MOUSEWHEEL:
            //     break;

        // case SDL_JOYDEVICEADDED:
        //     SDL_Log("handleEvent SDL_JOYDEVICEADDED\n");
        //     break;
        case SDL_JOYAXISMOTION:
            // SDL_Log("SDL_JOYAXISMOTION which %d axis %d value %d pad1 %d pad2 %d pad3 %d pad4 %d\n",
            //     event.jaxis.which, event.jaxis.axis, event.jaxis.value,
            //     event.jaxis.padding1, event.jaxis.padding2, event.jaxis.padding3, event.jaxis.padding4);
            return handleAxisEvent(&event.jaxis);
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            // SDL_Log("SDL_JOYBUTTON \n");
            SDL_Log("SDL_JOYBUTTON which %d button %d state %d pad1 %d pad2 %d\n",
                event.jbutton.which, event.jbutton.button, event.jbutton.state,
                event.jbutton.padding1, event.jbutton.padding2);
            break;

        case SDL_CONTROLLERDEVICEADDED:
            // SDL_Log("SDL_CONTROLLERDEVICEADDED\n");
            initGameController();
            break;
        // case SDL_CONTROLLERAXISMOTION:
        //     SDL_Log("SDL_CONTROLLERAXISMOTION\n");
        //     break;
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            return handleController(&event.cbutton);
        }
    }

    return true;
}

SDL_AudioDeviceID initAudio(SDL_AudioCallback callback)
{
    SDL_AudioSpec spec, aspec;

    SDL_zero(spec);
    spec.freq = SAMPLE_RATE;
    spec.format = APP_AUDIO_FORMAT;
    spec.channels = APP_CHANNELS;
    spec.samples = APP_AUDIO_CHUNK;
    spec.callback = callback;
    spec.userdata = NULL;

    SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(NULL, 0, &spec, &aspec, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (audioDevice <= 0) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return false;
    }

    SDL_Log("aspec freq %d channel %d sample %d format %d", aspec.freq, aspec.channels, aspec.samples, aspec.format);

    // Start playing, "unpause"
    SDL_PauseAudioDevice(audioDevice, 0);
    return audioDevice;
}

#endif