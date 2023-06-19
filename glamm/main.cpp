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

#include <GL/glew.h>
#include <GL/glut.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cmath>
#include <cstdlib>

float _r = 1.0f;
float _g = 0.5f;
float _b = 0.0f;

std::chrono::time_point<std::chrono::system_clock> _ts;

float _vertices[] = { -1.0f, -1.0f, 0.0f, 1.0f,  -1.0f, 0.0f,
                      1.0f,  1.0f,  0.0f, -1.0f, 1.0f,  0.0f };

unsigned int _shader_program = 0;
unsigned int _vao = 0, _vbo = 0;

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

  glViewport(0, 0, 100, 100);

  glUseProgram(_shader_program);
  glBindVertexArray(_vao);
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
  std::ifstream ifs("shaders/glamm.vert", std::ios::binary);
  std::string src;
  src.assign(std::istreambuf_iterator<char>(ifs),
             std::istreambuf_iterator<char>());
  const char* vsc = src.c_str();
  ifs.close();

  unsigned int v_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(v_shader, 1, &vsc, nullptr);
  glCompileShader(v_shader);

  // vertex shader error checking
  int glsuccess;
  char info_log[512];
  glGetShaderiv(v_shader, GL_COMPILE_STATUS, &glsuccess);

  if (!glsuccess) {
    glGetShaderInfoLog(v_shader, 512, nullptr, info_log);
    std::cerr << "Vertex shader compilation failed! " << info_log << std::endl;
    glDeleteShader(v_shader);
    return EXIT_FAILURE;
  }

  // read in fragment shader
  ifs.open("shaders/glamm.frag", std::ios::binary);
  src.assign(std::istreambuf_iterator<char>(ifs),
             std::istreambuf_iterator<char>());
  ifs.close();
  const char* f_src = src.c_str();

  unsigned int f_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(f_shader, 1, &f_src, nullptr);
  glCompileShader(f_shader);

  glGetShaderiv(f_shader, GL_COMPILE_STATUS, &glsuccess);

  if (!glsuccess) {
    glGetShaderInfoLog(f_shader, 512, nullptr, info_log);
    std::cerr << "Fragment shader compilation failed! " << info_log
              << std::endl;
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
    return EXIT_FAILURE;
  }

  // create shader program
  _shader_program = glCreateProgram();
  glAttachShader(_shader_program, v_shader);
  glAttachShader(_shader_program, f_shader);
  glLinkProgram(_shader_program);

  glGetProgramiv(_shader_program, GL_LINK_STATUS, &glsuccess);
  if (!glsuccess) {
    glGetProgramInfoLog(_shader_program, 512, nullptr, info_log);
    std::cerr << "Shader program failed to link! " << info_log << std::endl;
    return EXIT_FAILURE;
  }

  glDeleteShader(v_shader);
  glDeleteShader(f_shader);

  // setup
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