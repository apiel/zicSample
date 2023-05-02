# TODO

- standardise layout to be 4 label value per row
  BUT should there even be this top bar edit mode, there is 4 button (A,B,X,Y) giving the possibility to edit 2 values per button (even the main toggle button can have more feature if press long) and could also use double press. For fine tuning could use R1!!
- RG351P direction button repeat
- FIXME fix sample file selection: right now when pressing the edit button sample get loaded even if not changing sample. However, would be just better to open a modal to select file!

- LFO for filter (and maybe more)
- sample pitch
- grid/scatter effect
- negative resonance could do something else, maybe make a resoance with envelop?
- remove delay, reverbe, distortion and use instead send fx1, fx2 (and maybe use supercollider or libpd to apply fx)
- copy, paste, select
- github to save pattern
- FIXME fix filter click when switching quickly between value, next to 0% HPF

### Freesound

- use password auth to download wav file `curl -X POST -d "client_id=CLIENT_ID&client_secret=CLIENT_SECRET&grant_type=password&username=UNAME_OR_EMAIL&password=PASSWORD" https://freesound.org/apiv2/oauth2/access_token/`
- show loading data...
- improve UI: info on play/stop, download
- allow hi quality download (need oauth or direct grant)
- save file as
- tag file for category

### pitch shifting

Look at libsoundtouch for pitch shifting
https://codeberg.org/soundtouch/soundtouch/src/branch/master/include/SoundTouch.h
https://www.surina.net/soundtouch/README.html

Dont know if it can be done live, however we could simply preprocess every sample and allow +12 -12 semitone...

### synth mode

Should there be synth feature?
- supercollider, would require jack
- libpd, can only be one track
- custom synth...?

Should synth be live, or only to generate sample...? (but at least there could be one track that can then be recorded...)

If custom synth, simply use wavetable, then there would still be an (2?) LFO to morph over the wavetable (the same as filter LFO).
Finally, there would be a wavetable generator view, to create wavetable into file to reuse in the sequencer. The generator would combine 2 wavetables and 2 LFO, one LFO would be the master LFO to define the 64 morph steps of the new wavetable...

Drum synth engine to generate samples...
