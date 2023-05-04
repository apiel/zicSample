#ifndef _VIEW_MAIN_HEADER_TRACK_H
#define _VIEW_MAIN_HEADER_TRACK_H

#include "data.h"
#include "draw.h"
#include "drawHeaderButtonValue.h"

class ViewMainHeaderTrack {
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
};

#endif