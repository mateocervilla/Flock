# Flock
A simple C++ + SFML Flocking simulator

![flocking](https://github.com/mateocervilla/Flock/assets/68401037/3d02cada-69ff-4d3c-83ed-a302b6f11404)

## Table of contents
* [Requirements](#requirements)
* [Build](#build)
* [Run](#run)

## Requirements

This project uses **Conan** to manage all C++ dependencies.

You only need:

- **C++ compiler**
  - Linux: `gcc` or `clang`
  - Windows: MinGW or MSVC
- **CMake ≥ 3.15**
- **Conan ≥ 2.0**
- **Make** (Linux) or a supported build system

## Configure Conan profile (first time only)
```bash
conan profile detect --force
```

## Build
Static build (default)
```bash
conan build . --build=missing
```

Shared build
```bash
conan build . -o shared=True --build=missing
```

## Run
From the build directory:
* `./Flock` or double click on the icon.
