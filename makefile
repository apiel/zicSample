# for FTP EOF
.ONESHELL:

# default
CC=g++ -o zicSample

LIDSND=`pkg-config --cflags --libs sndfile`

SDL2=`sdl2-config --cflags --libs`
SDL2_TTF=`pkg-config --cflags --libs SDL2_ttf`

#CURL=`curl-config --cflags --libs`
CURL=`pkg-config --cflags --libs libcurl`

# FREESOUND_OAUTH=-DFREESOUND_OAUTH_ENABLED
FREESOUND=$(CURL) -DFREESOUND_ENABLED $(FREESOUND_OAUTH)

RG351P=-ldl `pkg-config wayland-client --cflags --libs` -lasound -lpulse -lX11 -lXext -lXdamage -lXfixes -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread  -lwayland-egl -lwayland-cursor -lxkbcommon -lXss

BUILD=-Wall zicSample.cpp -fopenmp -Wno-narrowing $(SDL2) $(SDL2_TTF) $(LIDSND) $(FREESOUND)

RG351_IP=192.168.1.105

ifneq ($(shell uname -m),x86_64)
RPI := -DIS_RPI=1
endif

linux: build run

rg351:
	make rg351MountChroot rg351Next

rg351Scp:
	make rg351Next
	sshpass -p "ark" scp zicSampleRG351 ark@$(RG351_IP):/home/ark/zicSample/zicSampleRG351
	sshpass -p "ark" ssh ark@$(RG351_IP) tail -f /home/ark/zicSample/zic.log

rg351Next:
	sudo chroot /mnt/data/arm64_2/ /bin/bash -c "cd /home/alex/zicSample && make rg351Chrooted"

rg351MountChroot:
	sudo mount --bind /home/alex/Music/zicSample /mnt/data/arm64_2/home/alex/zicSample

rg351Chrooted:
	make CC="g++ -o zicSampleRG351" build

build:
	$(CC) $(BUILD) $(RPI)

build-RG351P:
	$(CC) $(BUILD) $(RG351P) $(RPI)

# -fopenmp for soundTouch "warning: ignoring #pragma omp parallel"
# -Wno-narrowing for font.h:62:56: warning: narrowing conversion of ‘(8 * ((int)size))’ from ‘int’ to ‘Uint16’ {aka ‘short unsigned int’} inside { } [-Wnarrowing]

run:
	echo start zicSample
	LD_LIBRARY_PATH=. ./zicSample
