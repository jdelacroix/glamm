cmake_minimum_required(VERSION 3.12)

project(glamm)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

include_directories(
  ${OPENGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
)

add_executable(
  ${PROJECT_NAME}
  glamm/main.cpp
  # library
  glamm/draw_map_shader.cpp
  glamm/frame_buffer.cpp
  glamm/occupancy_grid_texture_map.cpp
  glamm/render_merged_map_shader.cpp
  glamm/shader_program.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARIES}
  ${GLEW_LIBRARIES}
  # ${GLM_LIBRARIES} header only
)

# shaders

set(
  GLAMM_SHADERS
    draw_map
    blit_maps
    render_merged_map
)

foreach(SHADER IN LISTS GLAMM_SHADERS)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/glamm/shaders/${SHADER}.vs ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER}.vs COPYONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/glamm/shaders/${SHADER}.fs ${CMAKE_CURRENT_BINARY_DIR}/shaders/${SHADER}.fs COPYONLY)
endforeach()


