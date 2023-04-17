#!/bin/bash

cd /roms/ports/zicSample
#SDL_AUDIODRIVER=pulseaudio 
#sudo chown -R $USER:$USER $HOME/
export XDG_RUNTIME_DIR="" 
LD_LIBRARY_PATH=. ./zicSample > zic.log 2>&1
