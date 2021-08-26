# Raylib+Flecs Starter Kit

This is a template repository that makes it easy to get up-and-running with
Raylib, Flecs, cJSON and Chipmunk.

Builds for Windows, MacOS and Linux are available here:

https://kranzky.itch.io/raylib-flecs-starter-kit

## Building

You can run the project in VSCode. From the command-line, it's just:

```
> make
```

## Releasing

You can release your project to itch.io as follows:

```
> make BUILD=RELEASE clean
> make BUILD=RELEASE
> make BUILD=RELEASE dist ITCHUSER=youruser ITCHGAME=yourgame
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
