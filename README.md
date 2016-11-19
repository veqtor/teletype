# teletype

[![Build Status](https://travis-ci.org/monome/teletype.svg?branch=master)](https://travis-ci.org/monome/teletype)

monome eurorack module

http://monome.org/docs/modular/teletype

## Directories

- `src`: source code for the teletype algorithm
- `module`: `main.c` and additional code for the Eurorack module (e.g. IO and UI)
- `tests`: algorithm tests
- `simulator`: a (very) simple teletype command parser and simulator

## Building

See the [libavr32 repo][libavr32] for more detailed instructions. You will also need `ragel` installed and on the path, see below.

**Make sure that the `libavr32` submodule is correctly checked out**

```bash
cd module
make clean
make
./flash.sh
```

## Ragel

The [Ragel state machine compiler][ragel] is required to build the firmware. It needs to be installed and on the path:

```bash
brew install ragel  # Homebrew (OSX)
apt install ragel   # Debian / Ubuntu / WSL (Windows 10)
pacman -Sy ragel    # Arch Linux / MSYS2 (Windows)
```

Version 6.9 is known to work.

See section 6.3 in the Ragel manual for information on the `=>` scanner constructor used.

## Code Formatting

To format the code using `clang-format`, run `make format` in the project's root directory. This *shouldn't* format any code in the `libavr32` submodule.

[libavr32]: https://github.com/monome/libavr32
[ragel]: http://www.colm.net/open-source/ragel/
