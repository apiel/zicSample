#ifndef _VIEW_MAIN_TRACK_H
#define _VIEW_MAIN_TRACK_H

#include "data.h"
#include "draw.h"
#include "drawHeaderButtonValue.h"

class ViewMainTrack {
protected:
    HeaderButtonValue headerButtonValue = HeaderButtonValue();

public:
    void renderY(Track& track)
    {
        headerButtonValue.btnY.label1 = "Volume";
        char volume[4];
        sprintf(volume, "%d", (int)(track.volume * 100));
        headerButtonValue.btnY.value1 = volume;
        headerButtonValue.btnY.unit1 = "%";

        headerButtonValue.btnY.label2 = "Distortion";
        char distortion[4];
        sprintf(distortion, "%d", (int)(track.distortion.drive * 100));
        headerButtonValue.btnY.value2 = distortion;
        headerButtonValue.btnY.unit2 = "%";

        headerButtonValue.drawY();
    }

    void renderX(Track& track)
    {
        headerButtonValue.btnX.label1 = "Sample";
        headerButtonValue.btnX.value1 = track.audioFileName;

        headerButtonValue.drawX();
    }

    void renderA(Track& track)
    {
        headerButtonValue.btnA.label1 = track.filter.getName();
        char filter[6];
        sprintf(filter, "%.1f", track.filter.getPctValue());
        headerButtonValue.btnA.value1 = filter;
        headerButtonValue.btnA.unit1 = "%";

        headerButtonValue.btnA.label2 = "Resonance";
        char resonance[4];
        sprintf(resonance, "%d", (int)(track.filter.resonance * 100));
        headerButtonValue.btnA.value2 = resonance;
        headerButtonValue.btnA.unit2 = "%";

        headerButtonValue.drawA();
    }

    void renderB(Track& track)
    {
        headerButtonValue.btnB.label1 = "Status";
        headerButtonValue.btnB.value1 = track.active ? "Playing" : "Stopped";

        // headerButtonValue.btnB.label2 = "Velocity";
        // headerButtonValue.btnB.value2 = "100";

        headerButtonValue.drawB();
    }

    void render(Track& track)
    {
        drawText({ 10, 10 }, track.name, COLOR_INFO);

        renderY(track);
        renderX(track);
        renderA(track);
        renderB(track);
    }

    void renderName(Track& track, unsigned int y)
    {
        drawFilledRect({ 5, y }, { 84, 12 }, COLOR_FOREGROUND);

        SDL_Color trackColor = COLOR_FOREGROUND2;
        SDL_Color trackText = COLOR_INFO;
        if (track.active) {
            trackColor = COLOR_ON;
            trackText = COLOR_WHITE;
        }
        trackColor.a = 50;
        drawFilledRect({ 5, y }, { 84, 12 }, trackColor);
        trackColor.a = 200;
        unsigned int width = 84.0 * track.volume;
        drawFilledRect({ 5, y }, { width, 12 }, trackColor);

        drawText({ 8, y }, track.name, trackText, 10);
    }

    bool handle(UiKeys& keys, Track& track, unsigned int y)
    {
        if (keys.btnB) {
            track.toggle();
            renderB(track);
            renderName(track, y);
            draw();
            return true;
        } else if (keys.btnA) {
            if (keys.isVertical()) {
                track.filter.set(track.filter.value + keys.getVertical(10, 50));
                renderA(track);
                draw();
                return true;
            } else if (keys.isHorizontal()) {
                track.filter.setResonance(track.filter.resonance + keys.getHorizontal(0.01, 0.05));
                renderA(track);
                draw();
                return true;
            }
        } else if (keys.btnX) {
            printf("keys.btnX\n");
            track.setNextAudioFileName(keys.getOneDirection());
            renderX(track);
            draw();
            return true;
        } else if (keys.btnY) {
            if (keys.isVertical()) {
                track.setVolume(track.volume + keys.getVertical(0.05, 0.01));
                renderY(track);
                renderName(track, y);
                draw();
                return true;
            } else if (keys.isHorizontal()) {
                track.distortion.set(track.distortion.drive + keys.getHorizontal(0.01, 0.05));
                renderY(track);
                draw();
                return true;
            }
        }
        return false;
    }
};

#endif