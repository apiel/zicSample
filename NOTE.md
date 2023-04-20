# TODO

- move volume on track toggle
- standardise layout to be 4 label value per row
- remove delay, reverbe, distortion and use instead send fx1, fx2 (and maybe use supercollider or libpd to apply fx)
- make a unique filter with (left) hipass / lowpass (right) where center is no filtering, and maybe use similar concept with resonance, where postive value is normal resonnance, but negative could be something else, maybe distortion?
- load, save and save as pattern
- copy, paste, select
- menu to exit

- maybe the volume bar can be selected from the none edit mode and this would display the master parameters like BPM, master filter, fx1, fx2. (in another way, it could be great to have a view where everything is visible in one screen, so maybe remove unecessary thing in a menu, like bpm..., bpm could be changed by selecting the progress bar!!)

- use jostick to apply master fitler, and track filter and also use L1 R1... to apply grid effect




### pitch shifting

Look at libsoundtouch for pitch shifting
https://codeberg.org/soundtouch/soundtouch/src/branch/master/include/SoundTouch.h
https://www.surina.net/soundtouch/README.html

Dont know if it can be done live, however we could simply preprocess every sample and allow +12 -12 semitone...

### Download samples

Use freesound api: https://freesound.org/help/developers/#freesound-api

### synth mode

Should there be synth feature?
- supercollider, would require jack
- libpd, can only be one track
- custom synth...?

Should synth be live, or only to generate sample...? (but at least there could be one track that can then be recorded...)

### sdl mixer ?

Alternative would be to use SDL mixer?
https://wiki.libsdl.org/SDL2_mixer/FrontPage
https://thenumb.at/cpp-course/sdl2/06/06.html
https://lazyfoo.net/tutorials/SDL/21_sound_effects_and_music/index.php
