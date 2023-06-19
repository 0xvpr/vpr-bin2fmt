TARGET      = vpr-bin2fmt

CC          = $(TOOLCHAIN_PREFIX)gcc
CFLAGS      = -O3 -Wall -Wextra -Werror -Wshadow -Wpedantic -Wconversion\
              -ffunction-sections -ffast-math -funroll-loops -fPIE

LD          = $(TOOLCHAIN_PREFIX)gcc
LDFLAGS     = -s

BIN         = bin
BUILD       = build

SOURCE      = src
OBJECT      = $(BUILD)
SOURCES     = $(wildcard $(SOURCE)/*.c)
REL_OBJECTS = $(patsubst $(SOURCE)/%.c,$(OBJECT)/%.o,$(SOURCES))
DBG_OBJECTS = $(patsubst $(SOURCE)/%.c,$(OBJECT)/%_d.o,$(SOURCES))

INCLUDE     = include
INCLUDES    = $(addprefix -I,$(INCLUDE))

ifeq ($(PREFIX),)
PREFIX      = /usr/local
endif

all: $(TARGET)
$(TARGET): $(BIN)/$(PROJECT)

$(BIN)/$(PROJECT): $(BIN) $(BUILD) $(REL_OBJECTS)
	$(LD) $(LDFLAGS) $(REL_OBJECTS) -o $(BIN)/$(TARGET)

$(REL_OBJECTS): $(OBJECT)/%.o : $(SOURCE)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BIN):
	mkdir -p $@

$(BUILD):
	mkdir -p $@

.PHONY: install
install: $(BIN)/$(PROJECT)
	install -d $(PREFIX)/bin
	install -m 555 $(BIN)/$(TARGET) $(PREFIX)/bin

.PHONY: clean
clean:
	rm -f ./bin/*
	rm -f ./build/*

.PHONY: extra-clean
extra-clean:
	rm -fr ./bin
	rm -fr ./build
