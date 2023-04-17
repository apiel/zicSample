# for FTP EOF
.ONESHELL:

# default
CC=g++ -o zicSample

# Chroot into to build environment, then build libsndfile following the instructions here
# https://github.com/libsndfile/libsndfile#building-from-command-line
# or try to link lib like:
# LIDSND=-L . -I ./libsndfile/include -lsndfile
# LIDSND=`pkg-config --cflags --libs sndfile`
# or could use libaudiofile https://github.com/mpruett/audiofile
# seem to be easier to install in chroot


SDL2=`sdl2-config --cflags --libs`
SDL2_TTF=`pkg-config --cflags --libs SDL2_ttf`

RG351P=-ldl `pkg-config wayland-client --cflags --libs` -lasound -lpulse -lX11 -lXext -lXdamage -lXfixes -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread  -lwayland-egl -lwayland-cursor -lxkbcommon -lXss

BUILD=-Wall zicSample.cpp -fopenmp -Wno-narrowing $(SDL2) $(SDL2_TTF) $(LIDSND)

linux: build run

rg351:
	make rg351MountChroot rg351Next

rg351Next:
	sudo chroot /mnt/data/arm64_2/ /bin/bash -c "cd /home/alex/zicSample && make rg351Chrooted"

rg351MountChroot:
	sudo mount --bind /home/alex/Music/zicSample /mnt/data/arm64_2/home/alex/zicSample

rg351Chrooted:
	make CC="g++ -o zicSampleRG351" build

build:
	$(CC) $(BUILD)

build-RG351P:
	$(CC) $(BUILD) $(RG351P)

# -fopenmp for soundTouch "warning: ignoring #pragma omp parallel"
# -Wno-narrowing for font.h:62:56: warning: narrowing conversion of ‘(8 * ((int)size))’ from ‘int’ to ‘Uint16’ {aka ‘short unsigned int’} inside { } [-Wnarrowing]

run:
	echo start zicSample
	LD_LIBRARY_PATH=. ./zicSample
