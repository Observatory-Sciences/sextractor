# SExtractor

[![Build Status](https://travis-ci.org/astromatic/sextractor.svg?branch=master)](https://travis-ci.org/astromatic/sextractor)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/sextractor/badge.svg)](https://scan.coverity.com/projects/sextractor "Coverity Badge")
[![Documentation Status](https://readthedocs.org/projects/sextractor/badge/?version=latest)](http://sextractor.readthedocs.io/en/latest/?badge=latest)

[SExtractor] stands for ``Source Extractor'': a software for extracting catalogs of sources from astronomical images.

Check out the on-line [documentation], the [official web page], and the [user forum].

[SExtractor]: http://astromatic.net/software/sextractor
[documentation]: http://sextractor.readthedocs.org
[official web page]: http://astromatic.net/software/sextractor
[user forum]: http://astromatic.net/forum/forumdisplay.php?fid=4

## Statically-linked Windows build with OpenBLAS

This is a fork of SExtractor with modifications to compile the program
statically on Linux targeting Windows using OpenBLAS as the BLAS/LAPACK
implementation.  It is also possible to build a statically linked version of
SExtractor for Linux using OpenBLAS.

Rather than using the original SExtractors autotools configuration, this
respository has been set up with some CMake files to control the build process.
This is all a bit janky so your mileage may vary.  Both builds (targeting
either Windows or Linux) are known to work on Fedora 33 with:

    ```
    > cmake --version
    cmake version 3.18.4
    > gcc --version
    gcc (GCC) 10.2.1 20201125 (Red Hat 10.2.1-9)
    > x86_64-w64-mingw32-gcc --version
    x86_64-w64-mingw32-gcc (GCC) 10.2.1 20200723 (Fedora MinGW 10.2.1-2.fc33)
    ```

### Linux Build

To use build for Linux, the CMake files will automatically download and build the
dependencies (OpenBLAS and fftw3) for you.  From this directory run:

    ```
    > mkdir build_linux && cd build_linux
    > cmake -DCMAKE_BUILD_TYPE=Release ..
    ...
    > make -j12
    ...
    > ls src/sex
    src/sex
    ```

OpenBLAS is configured to statically assume the target CPU is NEHALEM
(i.e. doesn't have support for AVX).  This is a conservative assumption.

### Windows Build

Unfortunately, to cross-compile for Windows is not as easy.

First, you will need the Mingw C, C++ and Fortran compilers.  On Fedora this
requires installing the following packages:

    ```
    mingw64-gcc
    mingw64-gcc-c++
    mingw64-gcc-gfortran
    ```

Second, you will need a "CMake toolchain file" for your system referencing these compilers. 
For example:

    ```
    # file: mibgw-w64-x84_64.cmake
    set(CMAKE_SYSTEM_NAME Windows)
    set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

    # cross compilers to use for C, C++ and Fortran
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
    set(CMAKE_Fortran_COMPILER ${TOOLCHAIN_PREFIX}-gfortran)
    set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

    # target environment on the build host system
    set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

    # modify default behavior of FIND_XXX() commands
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    ```

For some reason, at the moment building OpenBLAS with Mingw doesn't work using
their CMake files.  As a work around you should be able to build the
dependencies manually.  You will need to download and extract the following:

- [fftw3](http://www.fftw.org/fftw-3.3.9.tar.gz)
- [OpenBLAS](https://github.com/xianyi/OpenBLAS/releases/download/v0.3.13/OpenBLAS-0.3.13.tar.gz)

Here we assume you have the following directory setup:

    ```
    > tree
        $TOP 
        |-- OpenBLAS-0.3.13/
        |-- fftw-3.3.9/
        |-- sextractor/
        `-- mingw-w64-x86_64.cmake
    ```

You will need to create a root directory to install these libraries into:

    ```
    > mkdir -p $TOP/opt/mingw
    ```

For fftw3 we can use cmake so run the following the `$TOP/fftw3` directory:

    ```
    > pwd
    $TOP/fftw-3.3.9
    > mkdir build && cd build
    > cmake .. \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=$ROOT/opt/mingw
        -DCMAKE_TOOLCHAIN_FILE=$ROOT/mingw-w64-x86_64.cmake \
        -DBUILD_SHARED_LIBS=OFF \
        -DENABLE_FLOAT=ON
    ...
    > make all install -j12
    ...
    ```

For OpenBLAS we need to use their Makefile so run:

    ```
    > pwd
    $TOP/OpenBLAS-0.3.13
    > make -j12 \
        CC=x86_64-w64-mingw32-gcc \
        FC=x86_64-w64-mingw32-gfortran \
        HOSTCC=gcc \ 
        CROSS=1 \
        NO_SHARED=1 \
        TARGET=NEHALEM
    ...
    > make PREFIX=$ROOT/opt/mingw install
    ```

Finally we can now build SExtractor referencing these dependencies:

    ```
    > pwd
    $TOP/sextractor
    > mkdir build && cd build
    > cmake .. \
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_TOOLCHAIN_FILE=$ROOT/mingw-w64-x86_64.cmake
        -DLIB_PREFIX=$ROOT/opt/mingw
    ...
    > make -j12
    ...
    > ls src/sex.exe
    src/sex.exe
    ```
## Dependencies

To enable cross-compilation to Windows, an implementation of mmap/munmap is
included from https://code.google.com/archive/p/mman-win32/ .
