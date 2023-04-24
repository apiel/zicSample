#ifndef _APP_SDL2_H_
#define _APP_SDL2_H_

#include "def.h"
#include "state.h"

bool handleKeyboard(SDL_KeyboardEvent* event)
{
    bool isPressed = event->type == SDL_KEYDOWN;

    ui.keys.update = true;

    // SDL_Log("handleKeyboard %d\n", event->keysym.scancode);
    // SDL_Log("handleKeyboard %d\n", event->repeat);
    switch (event->keysym.scancode) {
    case SDL_SCANCODE_UP:
        ui.keys.Up = isPressed;
        break;
    case SDL_SCANCODE_DOWN:
        ui.keys.Down = isPressed;
        break;
    case SDL_SCANCODE_LEFT:
        ui.keys.Left = isPressed;
        break;
    case SDL_SCANCODE_RIGHT:
        ui.keys.Right = isPressed;
        break;
    case SDL_SCANCODE_S:
        ui.keys.Edit = isPressed;
        break;
    case SDL_SCANCODE_Z: // Y on German keyboard
        ui.keys.Edit2 = isPressed;
        break;
    case SDL_SCANCODE_A:
        ui.keys.Action = isPressed;
        break;
    case SDL_SCANCODE_Q:
        ui.keys.Mode = isPressed;
        break;
    case SDL_SCANCODE_SPACE:
        ui.keys.Menu = isPressed;
        break;
    case SDL_SCANCODE_ESCAPE:
        return false;
    default:
        ui.keys.update = false;
        break;
    }

    return true;
}

// On RG351P, some button trigger both SDL_CONTROLLERBUTTON and SDL_JOYBUTTON
// Let's focus on SDL_JOYBUTTON as SDL_CONTROLLERBUTTON doesn't work for all button
bool handleControllerButton(SDL_ControllerButtonEvent* event)
{
    // SDL_Log("handleController btn %d state %d type %d which %d pad1 %d pad2 %d\n",
    //     event->button, event->state, event->type, event->which, event->padding1, event->padding2);

    bool isPressed = event->state == SDL_PRESSED;
    ui.keys.update = true;

    switch (event->button) {
    case 11:
        ui.keys.Up = isPressed;
        break;
    case 12:
        ui.keys.Down = isPressed;
        break;
    case 13:
        ui.keys.Left = isPressed;
        break;
    case 14:
        ui.keys.Right = isPressed;
        break;
    default:
        ui.keys.update = false;
        break;
    }

    return true;
}

bool handleJoyButton(SDL_JoyButtonEvent* event)
{
    // SDL_Log("handleJoyButton btn %d state %d type %d which %d pad1 %d pad2 %d\n",
    //     event->button, event->state, event->type, event->which, event->padding1, event->padding2);

    bool isPressed = event->state == SDL_PRESSED;
    ui.keys.update = true;

    switch (event->button) {
    case 0: // A
        ui.keys.Edit = isPressed;
        break;
    case 1: // B
        ui.keys.Action = isPressed;
        break;
    case 2: // X
        SDL_Log("handleJoyButton btn X state %d\n", isPressed);
        ui.keys.Edit3 = isPressed;
        break;
    case 3: // Y
        ui.keys.Edit2 = isPressed;
        break;
    case 5: // R1 ?
        SDL_Log("handleJoyButton btn R1 state %d\n", isPressed);
        ui.keys.R1 = isPressed;
        break;
    case 11: // R2
        SDL_Log("handleJoyButton btn R2 state %d\n", isPressed);
        ui.keys.R2 = isPressed;
        break;
    case 4: // L1
        SDL_Log("handleJoyButton btn L1 state %d\n", isPressed);
        ui.keys.L1 = isPressed;
        break;
    case 10: // L2
        SDL_Log("handleJoyButton btn L2 state %d\n", isPressed);
        ui.keys.L2 = isPressed;
        break;
    case 6: // start
        ui.keys.Menu = isPressed;
        break;
    case 7: // select
        ui.keys.Mode = isPressed;
        break;
    case 8: // joystick left
        SDL_Log("handleJoyButton btn joystick left state %d\n", isPressed);
        ui.keys.joystickLeft = isPressed;
        break;
    case 9: // joystick right
        SDL_Log("handleJoyButton btn joystick right state %d\n", isPressed);
        ui.keys.joystickRight = isPressed;
        break;
    default:
        ui.keys.update = false;
        break;
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
        ui.joysticks.X1 = value;
        ui.joysticks.X1Changed = true;
        break;
    case 1: // up/down (left joystick)
        ui.joysticks.Y1 = value;
        ui.joysticks.Y1Changed = true;
        break;
    case 2: // left/right (right joystick)
        ui.joysticks.X2 = value;
        ui.joysticks.X2Changed = true;
        break;
    case 3: // up/down (right joystick)
        ui.joysticks.Y2 = value;
        ui.joysticks.Y2Changed = true;
        break;
    default:
        break;
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

    // TODO on RG351P, SDL_CONTROLLER doesn't automatically repeat like computer keyboard
    // Need to make a repeat system to simulate base on timing...

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
        case SDL_JOYAXISMOTION:
            return handleAxisEvent(&event.jaxis);
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            return handleJoyButton(&event.jbutton);
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            return handleControllerButton(&event.cbutton);
        case SDL_CONTROLLERDEVICEADDED:
            initGameController();
            break;
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