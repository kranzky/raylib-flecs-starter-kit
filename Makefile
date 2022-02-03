# vim: noexpandtab filetype=make

.PHONY: starterkit 
.PHONY: raylib 
.PHONY: flecs 
.PHONY: cjson 
.PHONY: chipmunk 
.PHONY: tinyfiledialogs 
.PHONY: nuklear 
.PHONY: dist 
.PHONY: clean 

TARGET = starterkit
RELEASE = alpha

RESDIR = res
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# for recursive make
MAKE = make

# compiler and default flags
CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -Wshadow -Wno-unused-parameter -Wno-switch
CFLAGS += -pedantic-errors -std=gnu11

# linker and default flags
LINKER = $(CC) -o
LFLAGS = -Wall -std=gnu11

# build specific settings
BUILD ?= DEBUG
ifeq ($(BUILD),DEBUG)
  CFLAGS += -g -DDEBUG
  LFLAGS += -g
endif
ifeq ($(BUILD),RELEASE)
  CFLAGS += -Ofast -ffast-math -DRELEASE -DNDEBUG
  TAG = $(shell git describe --tags $$(git rev-list --tags --max-count=1))
  HASH = $(shell git rev-parse --short HEAD)
endif

# architecture-specific settings
ARCH ?= MAC
ITCHARCH ?= mac
ifeq ($(ARCH),MAC)
  ITCHARCH = mac
  CFLAGS += -DOSX -mmacosx-version-min=10.9
  CFLAGS += -arch x86_64 -arch arm64
  LFLAGS += -framework CoreVideo -framework IOKit -framework Cocoa
  LFLAGS += -framework GLUT -framework OpenGL
  LFLAGS += -arch x86_64 -arch arm64
  ifeq ($(BUILD),RELEASE)
    BINDIR = mac/StarterKit.app/Contents/MacOS
    RESDIR = $(BINDIR)/res
  endif
endif
ifeq ($(ARCH),WIN)
  ITCHARCH = win
  MAKE = mingw32-make
  CFLAGS += -DWINDOWS
  LFLAGS += -static
  TARGET = starterkit.exe
  ifeq ($(BUILD),RELEASE)
    BINDIR = win
    RESDIR = $(BINDIR)/res
  endif
endif
ifeq ($(ARCH),LINUX)
  ITCHARCH = linux
  CFLAGS += -DLINUX
  LFLAGS += -lGL -lm -lpthread -ldl -lrt -lX11
  ifeq ($(BUILD),RELEASE)
    BINDIR = linux
    RESDIR = $(BINDIR)/res
    TEST = $(shell uname -m)
    TARGET = starterkit32
    ifeq ($(TEST),x86_64)
      TARGET = starterkit64
    endif
  endif
endif

# dependencies
DEPDIR := .d
CFLAGS += -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

SOURCES := $(wildcard $(SRCDIR)/*.c $(SRCDIR)/systems/*.c $(SRCDIR)/managers/*.c $(SRCDIR)/scenes/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h $(SRCDIR)/components/*.h $(SRCDIR)/systems/*.h $(SRCDIR)/managers/*.h $(SRCDIR)/scenes/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm = rm -f

POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

$(shell mkdir -p $(DEPDIR) >/dev/null)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
$(OBJDIR)/%.o : $(SRCDIR)/%.c $(DEPDIR)/%.d
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

starterkit: $(BINDIR)/$(TARGET)

raylib: ./vendor/raylib/libraylib.a
RAYLIB_SOURCES := $(wildcard ./vendor/raylib/*.c)
RAYLIB_OBJECTS := $(RAYLIB_SOURCES:%.c=%.o)
./vendor/raylib/libraylib.a: ;
	$(MAKE) -C ./vendor/raylib RAYLIB_SRC_PATH=. MACOSX_DEPLOYMENT_TARGET=10.9 PLATFORM=PLATFORM_DESKTOP
LFLAGS += -L./vendor/raylib -lraylib
LIBS += ./vendor/raylib/libraylib.a
INCLUDE += -I./vendor/raylib

flecs: ./vendor/flecs/libflecs.a
FLECS_SOURCES := $(wildcard ./vendor/flecs/*.c)
FLECS_OBJECTS := $(FLECS_SOURCES:%.c=%.o)
./vendor/flecs/%.o : ./vendor/flecs/%.c
	$(CC) $(CFLAGS) -c $< -o $@
./vendor/flecs/libflecs.a : $(FLECS_OBJECTS)
	$(AR) rcs ./vendor/flecs/libflecs.a $(FLECS_OBJECTS)
LFLAGS += -L./vendor/flecs -lflecs
LIBS += ./vendor/flecs/libflecs.a
INCLUDE += -I./vendor/flecs

cjson: ./vendor/cJSON/libcjson.a
CJSON_SOURCES := $(wildcard ./vendor/cJSON/*.c)
CJSON_OBJECTS := $(CJSON_SOURCES:%.c=%.o)
./vendor/cJSON/%.o : ./vendor/cJSON/%.c
	$(CC) $(CFLAGS) -c $< -o $@
./vendor/cJSON/libcjson.a : $(CJSON_OBJECTS)
	$(AR) rcs ./vendor/cJSON/libcjson.a $(CJSON_OBJECTS)
LFLAGS += -L./vendor/cJSON -lcjson
LIBS += ./vendor/cJSON/libcjson.a
INCLUDE += -I./vendor/cJSON

chipmunk: ./vendor/Chipmunk2D/libchipmunk.a
CHIPMUNK_SOURCES := $(wildcard ./vendor/Chipmunk2D/*.c)
CHIPMUNK_OBJECTS := $(CHIPMUNK_SOURCES:%.c=%.o)
./vendor/Chipmunk2D/%.o : ./vendor/Chipmunk2D/%.c
	$(CC) $(CFLAGS) -I./vendor/Chipmunk2D -c $< -o $@
./vendor/Chipmunk2D/libchipmunk.a: $(CHIPMUNK_OBJECTS)
	$(AR) rcs ./vendor/Chipmunk2D/libchipmunk.a $(CHIPMUNK_OBJECTS)
LFLAGS += -L./vendor/Chipmunk2D -lchipmunk
LIBS += ./vendor/Chipmunk2D/libchipmunk.a
INCLUDE += -I./vendor/Chipmunk2D/chipmunk -I./vendor/Chipmunk2D

tinyfiledialogs: ./vendor/tinyfiledialogs/libtinyfiledialogs.a
TINYFILEDIALOGS_SOURCES := $(wildcard ./vendor/tinyfiledialogs/*.c)
TINYFILEDIALOGS_OBJECTS := $(TINYFILEDIALOGS_SOURCES:%.c=%.o)
./vendor/tinyfiledialogs/%.o : ./vendor/tinyfiledialogs/%.c
	$(CC) $(CFLAGS) -c $< -o $@
./vendor/tinyfiledialogs/libtinyfiledialogs.a : $(TINYFILEDIALOGS_OBJECTS)
	$(AR) rcs ./vendor/tinyfiledialogs/libtinyfiledialogs.a $(TINYFILEDIALOGS_OBJECTS)
LFLAGS += -L./vendor/tinyfiledialogs -ltinyfiledialogs
LIBS += ./vendor/tinyfiledialogs/libtinyfiledialogs.a
INCLUDE += -I./vendor/tinyfiledialogs

nuklear: ./vendor/Nuklear/libnuklear.a
NUKLEAR_SOURCES := $(wildcard ./vendor/Nuklear/*.c)
NUKLEAR_OBJECTS := $(NUKLEAR_SOURCES:%.c=%.o)
./vendor/Nuklear/%.o : ./vendor/Nuklear/%.c
	$(CC) -DSTBRP_STATIC $(CFLAGS) -c $< -o $@
./vendor/Nuklear/libnuklear.a : $(NUKLEAR_OBJECTS)
	$(AR) rcs ./vendor/Nuklear/libnuklear.a $(NUKLEAR_OBJECTS)
LFLAGS += -L./vendor/Nuklear -lnuklear
LIBS += ./vendor/Nuklear/libnuklear.a
INCLUDE += -I./vendor/Nuklear

ifeq ($(ARCH),WIN)
  LFLAGS += -lopengl32 -lgdi32 -lole32 -lcomdlg32
endif

$(BINDIR)/$(TARGET): $(OBJECTS) $(LIBS)
	$(LINKER) $@ $(OBJECTS) $(LFLAGS)
ifeq ($(BUILD),RELEASE)
	cp -R res $(RESDIR)
	@echo $(TAG)-$(RELEASE)+$(HASH) > $(RESDIR)/VERSION
  ifeq ($(ARCH),MAC)
		chflags hidden $(RESDIR)
		lsregister -f mac/StarterKit.app
		otool -l mac/StarterKit.app/Contents/MacOS/starterkit
  endif
  ifeq ($(ARCH),WIN)
		attrib +s +h $(RESDIR)
  endif
endif

dist:
	butler push $(ITCHARCH) $(ITCHUSER)/$(ITCHGAME):$(ITCHARCH) --userversion-file $(RESDIR)/VERSION

clean:
	$(rm) $(wildcard $(DEPDIR)/**/*.d)
	$(rm) $(wildcard $(DEPDIR)/**/*.Td)
	$(rm) $(wildcard $(DEPDIR)/*.d)
	$(rm) $(wildcard $(DEPDIR)/*.Td)
	$(rm) $(OBJECTS)
	$(rm) $(CHIPMUNK_OBJECTS)
	$(rm) $(RAYLIB_OBJECTS)
	$(rm) $(FLECS_OBJECTS)
	$(rm) $(CJSON_OBJECTS)
	$(rm) $(TINYFILEDIALOGS_OBJECTS)
	$(rm) $(NUKLEAR_OBJECTS)
	$(rm) ./vendor/Chipmunk2D/libchipmunk.a
	$(rm) ./vendor/raylib/libraylib.a
	$(rm) ./vendor/flecs/libflecs.a
	$(rm) ./vendor/cJSON/libcjson.a
	$(rm) ./vendor/tinyfiledialogs/libtinyfiledialogs.a
	$(rm) ./vendor/Nuklear/libnuklear.a
	$(rm) $(BINDIR)/$(TARGET)
ifeq ($(BUILD),RELEASE)
	$(rm) -R $(RESDIR)
endif

include $(wildcard $(DEPDIR)/**/*.d)
