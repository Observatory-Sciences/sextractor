cmake_minimum_required(VERSION 3.14)

project(sextractor VERSION 2.19.5 LANGUAGES C)

set(LIB_PREFIX  "" CACHE STRING "Prefix where to find static libraries")

include(FetchContent)

if (NOT ${LIB_PREFIX} STREQUAL "")
    add_library(openblas INTERFACE IMPORTED)
    target_link_libraries(openblas INTERFACE ${LIB_PREFIX}/lib/libopenblas.a)
    target_include_directories(openblas INTERFACE ${LIB_PREFIX}/include)

    add_library(fftw3f INTERFACE IMPORTED)
    target_link_libraries(fftw3f INTERFACE ${LIB_PREFIX}/lib/libfftw3f.a)
    target_include_directories(fftw3f INTERFACE ${LIB_PREFIX}/include)
else()
    FetchContent_Declare(
        fftw
        URL http://fftw.org/fftw-3.3.9.tar.gz 
        URL_HASH MD5=50145bb68a8510b5d77605f11cadf8dc
        )

    FetchContent_GetProperties(fftw)
    if(NOT fftw_POPULATED)
        FetchContent_Populate(fftw)
        set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
        set(BUILD_TESTS OFF CACHE BOOL "")
        set(ENABLE_FLOAT ON CACHE BOOL "")
        set(DISABLE_FORTRAN ON CACHE BOOL "")
        add_subdirectory(${fftw_SOURCE_DIR} ${fftw_BINARY_DIR})
    endif()

    FetchContent_Declare(
        openblas
        GIT_REPOSITORY https://github.com/xianyi/OpenBLAS.git
        GIT_TAG v0.3.13
        )

    FetchContent_GetProperties(openblas)
    if(NOT openblas_POPULATED)
        FetchContent_Populate(openblas)
        set(TARGET NEHALEM CACHE STRING "")
        add_subdirectory(${openblas_SOURCE_DIR} ${openblas_BINARY_DIR})
    endif()
    target_link_options(openblas PRIVATE -static-libgfortran)
    set(LAPACKE_INCLUDE_DIR ${openblas_SOURCE_DIR}/lapack-netlib/LAPACKE/include)
endif()

add_library(FFTW3::fftw3f ALIAS fftw3f)
add_library(OpenBLAS::BLAS ALIAS openblas)

add_subdirectory(src)
