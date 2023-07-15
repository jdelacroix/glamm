cmake_minimum_required(VERSION 3.12)

project(glamm)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${OPENGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
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
  ${GLEW_LIBRARIES}
  # ${GLM_LIBRARIES} header only
)

add_executable(
  ${PROJECT_NAME}-viz
  bin/main.cpp
)

target_link_libraries(
  ${PROJECT_NAME}-viz
  # dependencies
  ${PROJECT_NAME}
  ${GLUT_LIBRARIES}
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
