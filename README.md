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

See the [libavr32 repo](https://github.com/monome/libavr32) for more detailed instructions.

**Make sure that the `libavr32` submodule is correctly checked out**

```bash
cd module
make clean
make
./flash.sh
```

## Code Formatting

To format the code using `clang-format`, run `make format` in the project's root directory. This *shouldn't* format any code in the `libavr32` submodule.
