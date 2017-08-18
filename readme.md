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

## Windows with VS2015
Pre-requisite : Visual Studio 2015 
                Qt Installation for Visual Studio 2015 (installed for exemple in c:/Qt/QtVersion/msvc2015_64/ refered as QTDIR in the next lines)

>     git clone git://github.com/crep4ever/benchmark-viewer.git
>     cd benchmark-viewer
>     mkdir build && cd build
>     cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_PREFIX_PATH="QTDIR"

Open the generated solution with Visual Studio and build the sources. The binary file will be generated in the TARGET_PATH (Debug/ or Release/).

To deploy the solution use the Qt deploy tool : http://doc.qt.io/qt-5/windows-deployment.html

> QTDIR/bin/windeployqt TARGET_PATH/



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
