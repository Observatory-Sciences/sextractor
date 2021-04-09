cmake_minimum_required(VERSION 3.14)

include(CheckIncludeFiles)

set(LIB_NAME fits)

file(GLOB ${LIB_NAME}_SOURCES
  fitsbody.c 
  fitscat.c 
  fitscheck.c 
  fitscleanup.c
  fitsconv.c 
  fitshead.c 
  fitskey.c 
  fitsmisc.c
  fitsread.c 
  fitstab.c 
  fitsutil.c 
  fitswrite.c
)

if (MINGW)
    list(APPEND ${LIB_NAME}_SOURCES mman/mman.c)
endif()

add_library(${LIB_NAME} STATIC ${${LIB_NAME}_SOURCES})
target_include_directories(${LIB_NAME} PRIVATE ${SEX_CONFIG_DIR})
add_library(SEX::FITS ALIAS ${LIB_NAME})

target_compile_definitions(${LIB_NAME} PRIVATE "HAVE_SYS_MMAN_H=1")

if (MINGW)
    target_include_directories(${LIB_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/mman)
endif()


if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${LIB_NAME} PRIVATE -fcommon -g)
endif()
