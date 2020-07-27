# raytracer

Implementation of Peter Shirley's [Ray Tracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) tutorial. Additional improvements were made on top of the example code provided. The key new feature is multithreading for improved performance. Other refactorings were performed to adhere to C++ best practices, improve modularity, and improve readability.

## Building the Code
Requires CMake 3.17 and a C++17-compliant compiler
1. Clone repository
1. `mkdir build`
1. `./compile.sh`

## Running the Ray Tracer
`./build/main image.ppm`
The executable requires one argument that specifies the filename. The only supported filetype is PPM (Portable Pixelmap Format).

## Autoformatting
`./format.sh`
Automatting using lang-format. See `.clang-format` for the specific formatting options used.

## Static Analysis
1. `./analyze.sh`
Static analysis is performed by clang-tidy. See `.clang-tidy` for the specific clang-tidy options used.
