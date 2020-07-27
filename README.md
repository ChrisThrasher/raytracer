# raytracer

Implementation of Peter Shirley's [Ray Tracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) tutorial. Additional improvements were made on top of the example code provided. The key new feature is multithreading for improved performance. Other refactorings were performed to adhere to C++ best practices, improve modularity, and improve readability.

## Building the Code
Requires CMake 3.17 and a C++17-compliant compiler
1. Clone repository
1. `mkdir build`
1. `./compile.sh`

## Running the Ray Tracer
1. `./build/main image.ppm`

## Autoformatting
Performed by clang-format
1. `./format.sh`

## Static Analysis
Performed by clang-tidy
1. `./analyze.sh`
