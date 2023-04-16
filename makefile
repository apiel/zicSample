# for FTP EOF
.ONESHELL:

# default
CC=g++ -o zicSample

# https://git-scm.com/book/en/v2/Git-Tools-Submodules
# TODO for RG351P, either the following line, or install libsndfile-dev in chroot, or build directly on device
#LIDSND=-L . -I ../../libsndfile/include -lsndfile -DZIC_USE_LIBSNDFILE=1
LIDSND=`pkg-config --cflags --libs sndfile` -DZIC_USE_LIBSNDFILE=1


linux: build run

build:
	$(CC) -Wall zicSample.cpp -fopenmp -Wno-narrowing `sdl2-config --cflags --libs` $(LIDSND)

# -fopenmp for soundTouch "warning: ignoring #pragma omp parallel"
# -Wno-narrowing for font.h:62:56: warning: narrowing conversion of ‘(8 * ((int)size))’ from ‘int’ to ‘Uint16’ {aka ‘short unsigned int’} inside { } [-Wnarrowing]

run:
	echo start zicSample
	LD_LIBRARY_PATH=. ./zicSample
