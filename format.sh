#!/bin/bash

clang-format -i `find src -name *.cpp` `find src -name *.h` `find test -name *.cpp`
