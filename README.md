# RayFT

This is a WIP library for rendering text at high-quality in Raylib using rasterization provided by the FreeType library.
Here is what needs to be done to compile and run the sample code:

## 1. Install Raylib and FreeType

Install both libraries and add them to the include path of your editor. On macOS, you can use brew to install both of them:
```
brew install raylib
brew install freetype
```

## 2. Compile and run

Compile including both libraries. For macOS:
```
mkdir -p out && cc -std=c99 $(find src -type f -name '*.c') $(pkg-config --libs --cflags raylib freetype2) -o out/main
```

This compile command will create an `out` directory if not present, and create the `main` executable under it. Run it like this (macOS):
```
./out/main
```

## Development stage

Right now, this is not much of a library, but rather an example program that shows how to use the FreeType + Raylib combination. I'll work on turning this into a reusable library that can be vendored into a project.