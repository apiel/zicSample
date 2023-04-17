# for FTP EOF
.ONESHELL:

# default
CC=g++ -o zicSample

# https://git-scm.com/book/en/v2/Git-Tools-Submodules
# TODO for RG351P, either the following line, or install libsndfile-dev in chroot, or build directly on device
#LIDSND=-L . -I ../../libsndfile/include -lsndfile
LIDSND=`pkg-config --cflags --libs sndfile`


SDL2=`sdl2-config --cflags --libs`
SDL2_TTF=`pkg-config --cflags --libs SDL2_ttf`

RG351P=-lasound -lpulse -lX11 -lXext -lXdamage -lXfixes -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread  -lwayland-egl -lwayland-cursor -lxkbcommon -lXss

BUILD=$(CC) -Wall zicSample.cpp -fopenmp -Wno-narrowing $(SDL2) $(SDL2_TTF) $(LIDSND)

linux: build run

build:
	$(BUILD)

build-RG351P:
	$(BUILD) $(RG351P)

# -fopenmp for soundTouch "warning: ignoring #pragma omp parallel"
# -Wno-narrowing for font.h:62:56: warning: narrowing conversion of ‘(8 * ((int)size))’ from ‘int’ to ‘Uint16’ {aka ‘short unsigned int’} inside { } [-Wnarrowing]

run:
	echo start zicSample
	LD_LIBRARY_PATH=. ./zicSample
