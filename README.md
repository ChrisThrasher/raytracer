# raytracer

Implementation of Peter Shirley's [Ray Tracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) tutorial. Additional improvements were made on top of the example code provided. The key new feature is multithreading for improved performance. Other refactorings were performed to adhere to C++ best practices, improve modularity, and improve readability.

The purpose of this project is not to provide a useful ray tracer but rather to be a proving ground for C++ best practices and various language concepts. If the end product happens to be a usable ray tracer then that's good too. Do not take this project as a starting point for writing your own ray tracer. Much of the core ray tracing logic was directly copied from Peter Shirley's example code and I cannot guarantee its quality.

## Building the Code
Requires CMake 3.17 and a C++17-compliant compiler
1. Clone repository
1. `mkdir build`
1. `cd build`
1. `cmake ..`
1. `make`

## Running the Ray Tracer
`./build/main image.ppm`

The executable requires one argument that specifies the filename. The only supported filetype is [PPM (Portable Pixelmap Format)](http://netpbm.sourceforge.net/doc/ppm.html).

## Running Automated Tests
1. `cd build`
1. `make test`

This will run all automated tests. The project must already be compiled before attempting this. See [test](test) directory for all test-related code.

## Autoformatting
[`./format.sh`](format.sh)

Automatting is performed by clang-format. See [`.clang-format`](.clang-format) for the specific formatting options used.

## Static Analysis
[`./analyze.sh`](analyze.sh)

Static analysis is performed by clang-tidy. See [`.clang-tidy`](.clang-tidy) for the specific clang-tidy options used.

## Supported Systems
All development was done in macOS Catelina using Apple Clang version 11.0. Not yet tested with any other compilers or operating systems.
