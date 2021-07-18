# Raylib+Flecs Starter Kit

## Building

```
make RAYLIB_SRC_PATH=.
```

* add butler deployment to makefile


## Releasing

### Mac

```
> cd ../raylib/src
> git checkout 3.7.0
> export MACOSX_DEPLOYMENT_TARGET=10.9
> make
> cd ../..flecs
> git checkout v2.3.2
> cd ../cJSON
> git checkout v1.7.14
> cd ../blattix
> make BUILD=RELEASE
> butler validate mac
> butler push mac thepunkcollective/blattix:mac-beta --userversion-file mac/Blattix.app/Contents/MacOS/res/VERSION
```

### Win

```
> cd ../raylib/src
> edit config.h to enable SUPPORT_BUSY_WAIT_LOOP
> mingw32-make PLATFORM=PLATFORM_DESKTOP
> cd ../../flecs
> git checkout v2.3.2
> cd ../blattix
> make BUILD=RELEASE
> butler validate win
> butler push win thepunkcollective/blattix:win-beta --userversion-file win/res/VERSION
```

### Linux

```
> (do this on the 32-bit and 64-bit VMs)
> cd ../raylib/src
> edit c99 to gnu99 and enable SUPPORT_BUSY_WAIT_LOOP
> mingw32-make PLATFORM=PLATFORM_DESKTOP
> cd ../../flecs
> git checkout v2.3.2
> cd ../blattix
> make BUILD=RELEASE
> (do this on the 32-bit VM)
> cp linux/blattix32 ~/windows
> (do this on the 64-bit VM)
> mv ~/windows/blattix32 linux
> butler validate linux
> butler push linux thepunkcollective/blattix:linux-beta --userversion-file linux/res/VERSION
```

## Copyright

Copyright (c) 2021 Jason Hutchens. See UNLICENSE for further details.
