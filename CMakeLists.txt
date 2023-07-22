cmake_minimum_required(VERSION 3.12)

project(glamm)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
# dependencies
pkg_check_modules(GLES REQUIRED glesv2)
pkg_check_modules(GLM REQUIRED glm)
pkg_check_modules(EGL REQUIRED egl)
pkg_check_modules(GBM REQUIRED gbm)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLES_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
  ${EGL_INCLUDE_DIRS}
  ${GBM_INCLUDE_DIRS}
)

add_library(
  ${PROJECT_NAME} STATIC
  # library
  glamm/blit_maps_shader.cpp
  glamm/frame_buffer.cpp
  glamm/map_merger.cpp
  glamm/occupancy_grid_texture_map.cpp
  glamm/pgm_io.cpp
  glamm/shader_program.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  # dependencies
  ${GLES_LIBRARIES}
  # ${GLM_LIBRARIES} # header only
)

add_executable(
  ${PROJECT_NAME}er
  bin/main.cpp
  glamm/virtual_display.cpp
)

target_link_libraries(
  ${PROJECT_NAME}er
  # dependencies
  ${PROJECT_NAME}
  ${EGL_LIBRARIES}
  ${GBM_LIBRARIES}
)

# shaders

set(
  GLAMM_SHADERS
    blit_maps
)

foreach(SHADER IN LISTS GLAMM_SHADERS)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/glamm/shaders/${SHADER}.vs ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER}.vs COPYONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/glamm/shaders/${SHADER}.fs ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER}.fs COPYONLY)
endforeach()

# maps from PGM
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/maps/example_map.pgm ${CMAKE_CURRENT_BINARY_DIR}/maps/example_map.pgm)
