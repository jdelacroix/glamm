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

float _vertices[] = { -50.0f, -50.0f, 0.0f, 50.0f,  -50.0f, 0.0f,
                      50.0f,  50.0f,  0.0f, -50.0f, 50.0f,  0.0f };

unsigned int _shader_program = 0;
unsigned int _vao = 0, _vbo = 0;

unsigned int _frame_buffers[2], _texture_buffers[2];

unsigned int _height = 200, _width = 200;

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
  glClear(GL_COLOR_BUFFER_BIT);

  // glViewport(0, 0, 100, 100);

  // glBindFramebuffer(GL_FRAMEBUFFER, _frame_buffers[0]);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture_buffers[0]);

  glUseProgram(_shader_program);
  glBindVertexArray(_vao);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glm::mat4 model(1.0f);
  float c = glm::sqrt(2.0f * 100.0f * 100.0f);
  model = glm::translate(model, glm::vec3(c, c, 0.0f));
  model =
    glm::rotate(model, glm::pi<float>() / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));

  glUniformMatrix4fv(glGetUniformLocation(_shader_program, "model"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(model));

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glFlush();
}

void
cycle_color()
{

  auto ts = std::chrono::system_clock::now();

  if (std::chrono::duration_cast<std::chrono::milliseconds>(ts - _ts) >=
      std::chrono::milliseconds(250)) {

    GLint url = glGetUniformLocation(_shader_program, "input_color");

    _r = fmod(_r + 0.1f, 1.0f);
    _g = fmod(_g + 0.1f, 1.0f);
    _b = fmod(_b + 0.1f, 1.0f);

    glUniform3f(url, _r, _g, _b);

    glm::mat4 model(1.0f);
    float c = glm::sqrt(2.0f * 50.0f * 50.0f);
    model = glm::translate(model, glm::vec3(c, c, 0.0f));
    model =
      glm::rotate(model, glm::pi<float>() / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 view(1.0f);

    glm::mat4 proj = glm::ortho(0.0f, 200.0f, 0.0f, 200.0f, 0.0f, 1.0f);

    glUniformMatrix4fv(glGetUniformLocation(_shader_program, "model"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(model));

    glUniformMatrix4fv(glGetUniformLocation(_shader_program, "view"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(view));

    glUniformMatrix4fv(glGetUniformLocation(_shader_program, "proj"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(proj));

    _ts = ts;

    glutPostRedisplay();
  }
}

int
main(int argc, char** argv)
{

  // initialize glut
  glutInit(&argc, argv);

  glutInitWindowSize(200, 200);
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
  glamm::ShaderProgram mapping("shaders/mapping.vs", "shaders/mapping.fs");
  _shader_program = mapping.id();

  // setup

  glamm::OccupancyGridTextureMap map(
    0.0, 0.0, 0.0, 100, 100); // remove before flight

  // create framebuffers

  glGenFramebuffers(2, _frame_buffers);
  glGenTextures(2, _texture_buffers);

  for (size_t i = 0; i < 2; ++i) {
    glBindFramebuffer(GL_FRAMEBUFFER, _frame_buffers[i]);
    glBindTexture(GL_TEXTURE_2D, _texture_buffers[i]);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA16F,
                 _width,
                 _height,
                 0,
                 GL_RGBA,
                 GL_FLOAT,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           _texture_buffers[i],
                           0);
  }

  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo);

  glBindVertexArray(_vao);

  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind vbo
  glBindVertexArray(0);             // unbind vao

  _ts = std::chrono::system_clock::now();
  glutMainLoop();

  return EXIT_SUCCESS;
}