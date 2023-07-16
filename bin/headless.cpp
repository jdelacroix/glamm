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

// same as main.cpp, but without glut

#include "glamm/blit_maps_shader.hpp"
#include "glamm/frame_buffer.hpp"
#include "glamm/occupancy_grid_texture_map.hpp"
#include "glamm/pgm_io.hpp"
#include "glamm/render_merged_map_shader.hpp"
#include "glamm/shader_program.hpp"
#include "glamm/virtual_display.hpp"

#include <epoxy/gl.h>
#include <gbm.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>

#include <cmath>
#include <cstdlib>

float _r = 0.31f;
float _g = 0.78f;
float _b = 0.47f;

std::chrono::time_point<std::chrono::system_clock> _ts;

unsigned int _width = 1000, _height = 1000;

std::unique_ptr<glamm::BlitMapsShader> _blit_maps_shader;
// std::unique_ptr<glamm::RenderMergedMapShader> _render_merged_map_shader;

std::unique_ptr<glamm::FrameBuffer> _front_frame_buffer, _back_frame_buffer;

GLfloat _map_texture_buffer[16384];
unsigned int _map_texture_id;

std::random_device _rd;
std::mt19937 _gen(_rd());
std::uniform_real_distribution<float> _distrib(0.0f,
                                               static_cast<float>(_width));
std::uniform_real_distribution<float> _distrib_yaw(-glm::pi<float>(),
                                                   glm::pi<float>());

void
abort_on_gl_error(const size_t line)
{
  GLenum glerr = glGetError();
  if (glerr != GL_NO_ERROR) {
    std::cerr << "(" << line << ") GL error: " << glerr << std::endl;
    exit(EXIT_FAILURE);
  }
}

int
main(int argc, char** argv)
{
  // initialize egl

  std::unique_ptr<glamm::VirtualDisplay> display;
  try {
    display = std::make_unique<glamm::VirtualDisplay>();
  } catch (const std::runtime_error& e) {
    std::cerr << "Unable to create virtual display: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  // initialize glew
  // GLenum glerr = glewInit();
  // if (glerr != GLEW_OK && glerr != GLEW_ERROR_NO_GLX_DISPLAY) {
  //   std::cerr << "GLEW is not OK! " << glerr << std::endl;
  //   return EXIT_FAILURE;
  // }

  _blit_maps_shader = std::make_unique<glamm::BlitMapsShader>(_width, _height);
  // _render_merged_map_shader =
  // std::make_unique<glamm::RenderMergedMapShader>();

  _front_frame_buffer = std::make_unique<glamm::FrameBuffer>(_width, _height);

  glamm::load_map_from_pgm(
    "maps/example_map.pgm", &_map_texture_buffer[0], 16384);

  _front_frame_buffer->activate();

  auto ts = std::chrono::system_clock::now();

  for (size_t i = 0; i < 10000; ++i) {

    glGenTextures(1, &_map_texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _map_texture_id);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA8,
                 8,
                 8,
                 0,
                 GL_RED,
                 GL_FLOAT,
                 &_map_texture_buffer[0]);
    abort_on_gl_error(__LINE__);
    glGenerateMipmap(GL_TEXTURE_2D);

    abort_on_gl_error(__LINE__);
    std::cout << "loaded: " << i << std::endl;
  }

  std::cout << "texture load time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::system_clock::now() - ts)
                 .count()
            << "ms" << std::endl;

  // create framebuffers
  _ts = std::chrono::system_clock::now();

  ts = std::chrono::system_clock::now();

  glViewport(0, 0, _width, _height);

  _front_frame_buffer->activate();

  glClearColor(0.5f, 0.5f, 0.5f, 0.0f); // gray
  glClear(GL_COLOR_BUFFER_BIT);

  for (size_t i = 0; i < 100; ++i) {
    const float c_x = 500.0f - _distrib(_gen), c_y = 500.0f - _distrib(_gen);
    glamm::OccupancyGridTextureMap map(c_x, c_y, _distrib_yaw(_gen), 100, 100);

    _blit_maps_shader->draw(
      map, _map_texture_id, _front_frame_buffer->texture_id());
  }

  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // glClear(GL_COLOR_BUFFER_BIT);

  std::cout << "benchmark: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::system_clock::now() - ts)
                 .count()
            << "ms" << std::endl;

  // ts = std::chrono::system_clock::now();
  // _render_merged_map_shader->draw(_front_frame_buffer->texture_id());

  // std::cout << "render time: "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(
  //                std::chrono::system_clock::now() - ts)
  //                .count()
  //           << "ms" << std::endl;

  ts = std::chrono::system_clock::now();

  GLubyte output_buffer[_width * _height];
  for (size_t i = 0; i < _width * _height; ++i) {
    output_buffer[i] = 0; // initialize to black
  }

  // NOT AVAILABLE IN OPENGL ES
  // glGetTextureImage(_front_frame_buffer->texture_id(),
  //                   0,
  //                   GL_RED,
  //                   GL_UNSIGNED_BYTE,
  //                   _width * _height,
  //                   &output_buffer[0]);

  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, _front_frame_buffer->texture_id());

  glReadnPixels(0,
                0,
                _width,
                _height,
                GL_RED,
                GL_UNSIGNED_BYTE,
                _width * _height,
                &output_buffer[0]);

  glamm::save_map_to_pgm(
    "output.pgm", _width, _height, &output_buffer[0], _width * _height);

  std::cout << "save time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::system_clock::now() - ts)
                 .count()
            << "ms" << std::endl;

  glFlush();

  return EXIT_SUCCESS;
}