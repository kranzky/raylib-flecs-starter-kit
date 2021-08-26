# vim: noexpandtab filetype=make

.PHONY: clean 
.PHONY: butler 

TARGET = starterkit
RELEASE = alpha

RESDIR = res
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# compiler and default flags
CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -Wshadow -Wno-unused-parameter
CFLAGS += -pedantic-errors -std=gnu11 -I./vendor/raylib -I./vendor/flecs -I./vendor/cJSON -I./vendor/Chipmunk2D

# linker and default flags
LINKER = $(CC) -o
LFLAGS = -Wall -std=gnu11 -L./vendor/raylib -lraylib -L./vendor/Chipmunk2D -lchipmunk

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
  LFLAGS += -framework CoreVideo -framework IOKit -framework Cocoa
  LFLAGS += -framework GLUT -framework OpenGL
  ifeq ($(BUILD),RELEASE)
    BINDIR = mac/StarterKit.app/Contents/MacOS
    RESDIR = $(BINDIR)/res
  endif
endif
ifeq ($(ARCH),WIN)
  ITCHARCH = win
  CFLAGS += -DWINDOWS
  LFLAGS += -static -lopengl32 -lgdi32
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

$(BINDIR)/$(TARGET): $(OBJECTS) ./vendor/Chipmunk2D/libchipmunk.a
	$(LINKER) $@ ./vendor/flecs/flecs.c ./vendor/cJSON/cJSON.c $(OBJECTS) $(LFLAGS)
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

$(OBJDIR)/%.o : $(SRCDIR)/%.c
$(OBJDIR)/%.o : $(SRCDIR)/%.c $(DEPDIR)/%.d
	$(CC) $(CFLAGS) -c $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

CHIPMUNK_SOURCES := $(wildcard ./vendor/Chipmunk2D/*.c)
CHIPMUNK_OBJECTS := $(CHIPMUNK_SOURCES:%.c=%.o)

./vendor/Chipmunk2D/%.o : ./vendor/Chipmunk2D/%.c
./vendor/Chipmunk2D/%.o : ./vendor/Chipmunk2D/%.c
	$(CC) $(CFLAGS) -c $< -o $@

./vendor/Chipmunk2D/libchipmunk.a: $(CHIPMUNK_OBJECTS)
	$(AR) rcs ./vendor/Chipmunk2D/libchipmunk.a $(CHIPMUNK_OBJECTS)

butler:
	butler push $(ITCHARCH) $(ITCHUSER)/$(ITCHGAME):$(ITCHARCH)-$(RELEASE) --userversion-file $(RESDIR)/VERSION

clean:
	$(rm) $(wildcard $(DEPDIR)/**/*.d)
	$(rm) $(wildcard $(DEPDIR)/**/*.Td)
	$(rm) $(wildcard $(DEPDIR)/*.d)
	$(rm) $(wildcard $(DEPDIR)/*.Td)
	$(rm) $(OBJECTS)
	$(rm) $(CHIPMUNK_OBJECTS)
	$(rm) ./vendor/Chipmunk2D/libchipmunk.a
	$(rm) $(BINDIR)/$(TARGET)
ifeq ($(BUILD),RELEASE)
	$(rm) -R $(RESDIR)
endif

include $(wildcard $(DEPDIR)/*.d)
