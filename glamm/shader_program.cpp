
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

#include "shader_program.hpp"

#include <epoxy/gl.h>

#include <fstream>
#include <iostream>

namespace glamm {

unsigned int
create_shader_program_from(const std::string& vertex_shader_url,
                           const std::string& fragment_shader_url)
{

  // read in vertex shader
  std::ifstream ifs(vertex_shader_url, std::ios::binary);
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
    // std::cerr << "Vertex shader compilation failed! " << info_log <<
    // std::endl;
    glDeleteShader(v_shader);
    // return EXIT_FAILURE;
    throw std::runtime_error("Vertex shader compliation failed: " +
                             std::string(info_log));
  }

  // read in fragment shader
  ifs.open(fragment_shader_url, std::ios::binary);
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
    // std::cerr << "Fragment shader compilation failed! " << info_log
    //           << std::endl;
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
    // return EXIT_FAILURE;
    throw std::runtime_error("Fragment shader compilation failed: " +
                             std::string(info_log));
  }

  // create shader program
  unsigned int id = glCreateProgram();
  glAttachShader(id, v_shader);
  glAttachShader(id, f_shader);
  glLinkProgram(id);

  glGetProgramiv(id, GL_LINK_STATUS, &glsuccess);
  if (!glsuccess) {
    glGetProgramInfoLog(id, 512, nullptr, info_log);
    std::cerr << "Shader program failed to link! " << info_log << std::endl;
    // return EXIT_FAILURE;
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
    throw std::runtime_error("Shader program failed to link: " +
                             std::string(info_log));
  }

  glDeleteShader(v_shader);
  glDeleteShader(f_shader);

  return id;
}

}