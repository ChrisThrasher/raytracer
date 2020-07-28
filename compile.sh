#!/bin/bash

export CTEST_OUTPUT_ON_FAILURE=1

cd build
cmake ../
make $1
