cmake_minimum_required(VERSION 3.12)

project(glamm)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
  ${OPENGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
)

add_executable(
  ${PROJECT_NAME}
  glamm/main.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARIES}
)