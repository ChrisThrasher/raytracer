# raytracer

Implementation of Peter Shirley's [Ray Tracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) tutorial. Additional improvements were made on top of the example code provided. The key new feature is multithreading for improved performance. Other refactorings were performed to adhere to C++ best practices, improve modularity, and improve readability.

The purpose of this project is not to provide a useful ray tracer but rather to be a proving ground for C++ best practices and various language concepts. If the end product happens to be a usable ray tracer then that's good too. Do not take this project as a starting point for writing your own ray tracer. Much of the core ray tracing logic was directly copied from Peter Shirley's example code and I cannot guarantee its quality.

# Usage
```
$ ./build/raytracer --help
Usage
  raytracer <output_filename>

Positions
  output_filename Output file name

Options
  -h, --help      Show this help text
```

The only supported filetype is [PPM (Portable Pixelmap Format)](http://netpbm.sourceforge.net/doc/ppm.html).
