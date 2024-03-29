# Zic Sample

Simple sample sequencer for Anbernic handled console.

<img src='https://github.com/apiel/zicSample/blob/main/screenshot/demo.png?raw=true' />

Here is a demo video from a previous version:

[![Demo video from a previous version](https://img.youtube.com/vi/Q-75PZpsoY8/0.jpg)](https://www.youtube.com/watch?v=Q-75PZpsoY8)

## Installation

On rg351, install arkos.

Then install:

`sudo apt-get install fonts-liberation2 libsdl2-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev libcurl4-gnutls-dev libsndfile1-dev libsndfile1`


> INFO: SDL video driver: KMSDRM_LEGACY
> INFO: SDL audio driver: alsa

> Font path might differ, have a look at `def.h`, base path is either `/usr/share/fonts/truetype/liberation` or `/usr/share/fonts/truetype/liberation2`

## Toolchain

### anbernic rg351

https://github.com/christianhaitian/arkos/wiki/Building

To create debian based chroots in a Linux environment.
These instructions are based on a Ubuntu 16 or newer install or VM.
install Prereqs:

```sh
sudo apt update
sudo apt install -y build-essential debootstrap binfmt-support qemu-user-static
```

Then install armhf and arm64 chroots:

sudo qemu-debootstrap --arch arm64 buster /mnt/data/arm64 http://deb.debian.org/debian/

Get into chroots:

For 64 bit Arm environment:

```sh
sudo chroot /mnt/data/arm64/
```

Once in chroot, we are in an environment that can run ARM 64-bit binaries. For example, we can check the architecture with:

```sh
uname -m
```

> aarch64

And then, you may install packages with apt-get. For example:

```sh
apt-get install vim
```

Helpful tools to install:

```sh
apt -y install build-essential git wget libdrm-dev python3 python3-pip python3-setuptools python3-wheel ninja-build libopenal-dev premake4 autoconf libevdev-dev ffmpeg libsnappy-dev libboost-tools-dev magics++ libboost-thread-dev libboost-all-dev pkg-config zlib1g-dev libpng-dev libsdl2-dev clang cmake cmake-data libarchive13 libcurl4 libfreetype6-dev libjsoncpp1 librhash0 libuv1 mercurial mercurial-common libgbm-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev libsndfile1 libsndfile1-dev libcurl4-gnutls-dev 
# apt -y install libcurl4-openssl-dev libcurl4-nss-dev
```


or using docker https://github.com/mica-angeli/retrooz_dev_docker
