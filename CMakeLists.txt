cmake_minimum_required(VERSION 3.12)

project(glamm)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
# find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(EGL REQUIRED egl)
pkg_check_modules(GBM REQUIRED gbm)
pkg_check_modules(EPOXY REQUIRED epoxy)
pkg_check_modules(GLES REQUIRED glesv2)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${OPENGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  # ${GLEW_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
  ${EGL_INCLUDE_DIRS}
  ${GBM_INCLUDE_DIRS}
  ${EXPOXY_INCLUDE_DIRS}
  ${GLES_INCLUDE_DIRS}
)

add_library(
  ${PROJECT_NAME} STATIC
  # library
  glamm/blit_maps_shader.cpp
  glamm/frame_buffer.cpp
  glamm/occupancy_grid_texture_map.cpp
  glamm/pgm_io.cpp
  glamm/render_merged_map_shader.cpp
  glamm/shader_program.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  # dependencies
  ${OPENGL_LIBRARIES}
  # ${GLEW_LIBRARIES}
  # ${GLM_LIBRARIES} header only
  ${EPOXY_LIBRARIES}
  ${GLES_LIBRARIES}
)

# add_executable(
#   ${PROJECT_NAME}-viz
#   bin/main.cpp
# )

# target_link_libraries(
#   ${PROJECT_NAME}-viz
#   # dependencies
#   ${PROJECT_NAME}
#   ${GLUT_LIBRARIES}
# )

add_executable(
  ${PROJECT_NAME}-headless
  bin/headless.cpp
  glamm/virtual_display.cpp
)

target_link_libraries(
  ${PROJECT_NAME}-headless
  # dependencies
  ${PROJECT_NAME}
  ${EGL_LIBRARIES}
  ${GBM_LIBRARIES}
)

# shaders

set(
  GLAMM_SHADERS
    blit_maps
    render_merged_map # viz only
)

foreach(SHADER IN LISTS GLAMM_SHADERS)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/glamm/shaders/${SHADER}.vs ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER}.vs COPYONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/glamm/shaders/${SHADER}.fs ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER}.fs COPYONLY)
endforeach()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/maps/example_map.pgm ${CMAKE_CURRENT_BINARY_DIR}/maps/example_map.pgm)
