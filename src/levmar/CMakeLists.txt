cmake_minimum_required(VERSION 3.14)

set(LIB_NAME levmar)

file(GLOB ${LIB_NAME}_SOURCES
  Axb.c 
  lm.c 
  lmbc.c 
  lmblec.c 
  lmbleic.c 
  lmlec.c 
  misc.c
)

add_library(${LIB_NAME} STATIC ${${LIB_NAME}_SOURCES})

target_include_directories(${LIB_NAME} PRIVATE ${LAPACKE_INCLUDE_DIR})

add_library(SEX::LEVMAR ALIAS ${LIB_NAME})

target_link_libraries(${LIB_NAME} PRIVATE OpenBLAS::BLAS)

if (NOT MSVC)
    target_link_libraries(${LIB_NAME} PUBLIC m)
endif()

