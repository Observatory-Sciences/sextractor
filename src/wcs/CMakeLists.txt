cmake_minimum_required(VERSION 3.14)

set(LIB_NAME wcs)

file(GLOB ${LIB_NAME}_SOURCES
  cel.c 
  lin.c 
  poly.c 
  proj.c 
  sph.c 
  tnx.c 
  wcs.c
  wcstrig.c 
)

add_library(${LIB_NAME} STATIC ${${LIB_NAME}_SOURCES})

target_link_libraries(${LIB_NAME} PRIVATE OpenBLAS::BLAS)

add_library(SEX::WCS ALIAS ${LIB_NAME})
