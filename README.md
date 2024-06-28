# Flock
A simple C++ + SFML Flocking simulator

![3dFlock](https://github.com/mateocervilla/3DFlock/assets/68401037/319ee0c3-08a6-42e3-bad9-6b9151b85b2c)

## Table of contents
* [Requirements](#requirements)
* [Build](#build)
* [Run](#run)

## Requirements
In order to build this project, you need the following dependencies:
* gcc
  * For Windows, use the MinGW compiler provided by SFML: [WinLibs MSVCRT 13.1.0 (64-bit)](https://github.com/brechtsanders/winlibs_mingw/releases/download/13.1.0-16.0.5-11.0.0-msvcrt-r5/winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64msvcrt-11.0.0-r5.7z)
* make
* cmake
  * [Windows](https://cmake.org/download/)
* SFML 2.6.1
  * [Linux](https://www.sfml-dev.org/files/SFML-2.6.1-linux-gcc-64-bit.tar.gz)
  * [Windows](https://www.sfml-dev.org/files/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit.zip)

Static linking libraries:
* Linux:
  * X11
    * `sudo apt-get install x11-xserver-utils`
  * Xrandr
    * `sudo apt-get install libxrandr-dev`
  * Xcursor
    * `sudo apt-get install libxcursor-dev`
  * udev
    * `sudo apt-get install libudev-dev`

* Windows (come with Windows by default):
  * opengl32
  * winmm
  * gdi32

## Build
* `cmake . -DSFML_PATH="path/path/to/sfml"`
> To build with shared libraries add -DSHARED=ON
* `make`

## Run
* `./Flock` or double click on the icon.
