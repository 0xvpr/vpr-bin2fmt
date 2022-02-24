PROJECT    = vpr-bin2fmt

CMAKE      = cmake

ifeq ($(TOOLCHAIN),)
TOOLCHAIN  = -DCMAKE_TOOLCHAIN_FILE="gcc-toolchain-release.cmake"
endif

BIN        = bin
SRC        = src
BUILD      = build
INCLUDE    = include

ifeq ($(PREFIX),)
PREFIX     = /usr/local
endif

all: release

.PHONY: build_release
build_release:
	cmake -B $(BUILD) $(TOOLCHAIN)

.PHONY: CMakeLists.txt
release: build_release
	cmake --build $(BUILD)

file:
	echo -n "abcdefgABCDEFG" > $@.txt

.PHONY: install
install:
	install -d $(PREFIX)/bin 
	install -m 555 $(BIN)/$(PROJECT) $(PREFIX)/bin

.PHONY: clean
clean:
	rm -fr bin/*
	rm -fr build/*
	rm -fr file.txt

.PHONY: extra-clean
extra-clean:
	rm -fr bin
	rm -fr build
	rm -fr file.txt
