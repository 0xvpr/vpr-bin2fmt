TARGET    = vpr-bin2fmt

CC        = gcc
CFLAGS    = -O3 -Wall -Wextra -Werror -Wshadow -Wpedantic -Wconversion\
            -ffunction-sections -ffast-math -funroll-loops -fPIE

LD        = gcc
LDFLAGS   = -s

BIN       = bin
BUILD     = build

SOURCE    = src
OBJECT    = $(BUILD)
SOURCES   = $(wildcard $(SOURCE)/*.c)
OBJECTS   = $(patsubst $(SOURCE)/%.c,$(OBJECT)/%.o,$(SOURCES))

INCLUDE   = include
INCLUDES  = $(addprefix -I,$(INCLUDE))

ifeq ($(PREFIX),)
PREFIX    = /usr/local
endif

all: $(TARGET)
$(TARGET): $(BIN)/$(PROJECT)

$(BIN)/$(PROJECT): $(BIN) $(BUILD) $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(BIN)/$(TARGET)

$(OBJECTS): $(OBJECT)/%.o : $(SOURCE)/%.c
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
	rm -fr ./$(BIN)/*
	rm -fr ./$(BUILD)/*

.PHONY: extra-clean
extra-clean:
	rm -fr ./$(BIN)
	rm -fr ./$(BUILD)
