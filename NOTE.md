# TODO

- save all params to file...

- sample pitch
- github to save pattern
- select sample R1 should switch to the next letter
- open a modal to select file playing sample ?
- LFO for filter, resonance, amplitude, and ?
- grid/scatter effect
- negative resonance could do something else, maybe make a resoance with envelop?
- master effect delay, reverb?, distortion?
- copy, paste, select?
- FIXME fix filter click when switching quickly between value, next to 0% HPF
- RG351P controller direction button repeat, incrementing slowly

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
