# Benchmark-Viewer ![BenchmarkViewer](icons/benchmark-viewer/48x48/benchmark-viewer.png)

Display C++ applications runtime as a timeline

[![Build Status](https://travis-ci.org/crep4ever/benchmark-viewer.svg?branch=master)](https://travis-ci.org/crep4ever/benchmark-viewer)
[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/9003.svg)](https://scan.coverity.com/projects/crep4ever-benchmark-viewer)

![BenchmarkViewer](doc/img/benchmark-viewer.png)

# Build and run

* required packages: cmake; qt5 (core, widgets);

## Linux

>     git clone git://github.com/crep4ever/benchmark-viewer.git
>     cd benchmark-viewer
>     mkdir build && cd build
>     cmake .. && make
>     sudo make install
>     cd -
>     benchmark-viewer data/example.csv

## CMake options

* CMAKE_BUILD_TYPE [ `Release` (default), `Debug`]

# Generate benchmark logs
The directory `examples` contains a minimal program example
that generates benchmark logs.

>     cd examples
>     g++ benchmark.cc main.cc
>     ./a.out # generates benchmark logs in session.csv
>     benchmark-viewer session.csv

![Example](examples/example.png)

# Contact
* romain.goffe@gmail.com
