cmake_minimum_required(VERSION 3.14)

add_subdirectory(levmar)
add_subdirectory(fits)
add_subdirectory(wcs)

set(EXE_NAME sex)

set(${EXE_NAME}_SOURCES 
  analyse.c 
  assoc.c 
  astrom.c 
  back.c 
  bpro.c
  catout.c 
  check.c
  clean.c
  dgeo.c
  extract.c 
  fft.c 
  field.c 
  filter.c 
  fitswcs.c
  flag.c 
  graph.c 
  growth.c 
  header.c 
  image.c 
  interpolate.c 
  main.c 
  makeit.c 
  manobjlist.c 
  misc.c 
  neurro.c 
  pattern.c 
  pc.c 
  photom.c 
  plist.c 
  prefs.c 
  profit.c 
  psf.c 
  readimage.c 
  refine.c 
  retina.c 
  scan.c 
  som.c 
  weight.c 
  winpos.c 
  xml.c 
)

add_executable(${EXE_NAME} ${${EXE_NAME}_SOURCES})

target_include_directories(${EXE_NAME} PRIVATE ${SEX_CONFIG_DIR} ${LAPACKE_INCLUDE_DIR})

target_link_libraries(${EXE_NAME} PRIVATE SEX::LEVMAR SEX::FITS SEX::WCS FFTW3::fftw3f)

target_compile_definitions(${EXE_NAME} PRIVATE
    "FFTW_H=\"fftw3.h\""
    "HAVE_LAPACKE=1"
    "LAPACKE_H=\"lapacke.h\""
    "USE_MODEL=1"
    "VERSION=\"2.19.5\""
    "DATE=\"2021-03-31\""
    "THREADS_NMAX=8"
    )

if (NOT MSVC)
    target_link_libraries(${EXE_NAME} PRIVATE m pthread)
endif()
