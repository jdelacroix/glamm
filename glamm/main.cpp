/*
 * Copyright 2023 Jean-Pierre de la Croix
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "frame_buffer.hpp"
#include "occupancy_grid_texture_map.hpp"
#include "shader_program.hpp"

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <cmath>
#include <cstdlib>

float _r = 1.0f;
float _g = 0.5f;
float _b = 0.0f;

std::chrono::time_point<std::chrono::system_clock> _ts;

unsigned int _draw_map = 0;
unsigned int _height = 800, _width = 800;

void
handle_key_event(unsigned char key, int x, int y)
{
  switch (key) {
    // escape key
    case '\x1B': {
      exit(EXIT_SUCCESS);
      break;
    }
  }
}

void
display()
{
  glViewport(0, 0, _width, _height);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white bg
  glClear(GL_COLOR_BUFFER_BIT);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glUseProgram(_draw_map);

  glamm::OccupancyGridTextureMap map(0.0f, 0.0f, 0.0f, 50.0f, 100.0f);

  glm::mat4 model = map.model();
  glUniformMatrix4fv(glGetUniformLocation(_draw_map, "model"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(model));

  glm::mat4 view(1.0f);
  view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));
  // view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

  glUniformMatrix4fv(
    glGetUniformLocation(_draw_map, "view"), 1, GL_FALSE, glm::value_ptr(view));

  glm::mat4 proj =
    glm::ortho(0.0f, (float)_width, 0.0f, (float)_height, 0.0f, 1.0f);

  glUniformMatrix4fv(
    glGetUniformLocation(_draw_map, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

  map.draw();

  glFlush();
}

void
cycle_color()
{

  auto ts = std::chrono::system_clock::now();

  if (std::chrono::duration_cast<std::chrono::milliseconds>(ts - _ts) >=
      std::chrono::milliseconds(250)) {

    GLint url = glGetUniformLocation(_draw_map, "input_color");

    _r = fmod(_r + 0.1f, 1.0f);
    _g = fmod(_g + 0.1f, 1.0f);
    _b = fmod(_b + 0.1f, 1.0f);

    glUniform3f(url, _r, _g, _b);

    _ts = ts;

    glutPostRedisplay();
  }
}

int
main(int argc, char** argv)
{
  // initialize glut
  glutInit(&argc, argv);

  glutInitWindowSize(_width, _height);
  glutCreateWindow("Hello, world!");
  glutKeyboardFunc(&handle_key_event);
  glutDisplayFunc(&display);
  glutIdleFunc(&cycle_color);

  // initialize glew
  GLenum glerr = glewInit();
  if (glerr != GLEW_OK) {
    std::cerr << "GLEW is not OK!" << std::endl;
    return EXIT_FAILURE;
  }

  // read in vertex shader
  glamm::ShaderProgram draw_map("shaders/draw_map.vs", "shaders/draw_map.fs");
  _draw_map = draw_map.id();

  // create framebuffers
  _ts = std::chrono::system_clock::now();
  glutMainLoop();

  return EXIT_SUCCESS;
}