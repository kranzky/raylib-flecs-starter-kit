# Raylib+Flecs Starter Kit

This is a template repository that makes it easy to get up-and-running with
Raylib and Flecs. Builds for Windows, MacOS and Linux are available here:

https://kranzky.itch.io/raylib-flecs-starter-kit

## Building

First you need to build the raylib library in `vendor/raylib` as follows:

```
> make clean
> make RAYLIB_SRC_PATH=. MACOSX_DEPLOYMENT_TARGET=10.9 PLATFORM=PLATFORM_DESKTOP
```

You will then be able to run the project in VS Code.

## Releasing

You can release your project to itch.io as follows:

```
> make BUILD=RELEASE clean
> make BUILD=RELEASE
> make BUILD=RELEASE butler ITCHUSER=youruser ITCHGAME=yourgame
```

On Windows, use `mingw32-make`. On Linux, you will release just the 32-bit or
64-bit executable, depending on your architecture. For maximum compatibility,
manually package up a release that supports both architectures.

## Configuration

Optionally change the name from "starterkit" to your own in these files:

```
.gitignore
.vscode/launch.json
.vscode/tasks.json
Makefile
src/managers/game.c
linux/launch
mac/.itch.toml
mac/StarterKit.app/Contents/Info.plist
mac/StarterKit.app/Contents/MacOS/launch
win/.itch.toml
```

## Copyright

Copyright (c) 2021 Jason Hutchens. See UNLICENSE for further details.
