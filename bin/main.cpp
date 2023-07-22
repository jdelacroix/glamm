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
#include "glamm/map_merger.hpp"
#include "glamm/occupancy_grid_texture_map.hpp"
#include "glamm/pgm_io.hpp"
#include "glamm/shader_program.hpp"
#include "glamm/virtual_display.hpp"

// #include <epoxy/gl.h>
#include <GLES3/gl31.h>

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
  } else {
    // std::cout << "GL_NO_ERROR!" << std::endl;
  }
}

int
main(int argc, char** argv)
{
  // initialize egl

  std::unique_ptr<glamm::VirtualDisplay> display;
  try {
    display = std::make_unique<glamm::VirtualDisplay>("/dev/dri/card0");
  } catch (const std::runtime_error& e) {
    std::cerr << "Unable to create virtual display: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  glamm::MapMerger map_merger(_width, _height);

  glamm::load_map_from_pgm(
    "maps/example_map.pgm", &_map_texture_buffer[0], 16384);

  auto ts = std::chrono::system_clock::now();

  for (size_t i = 0; i < 100; ++i) {
    const float c_x = 500.0f - _distrib(_gen), c_y = 500.0f - _distrib(_gen);
    glamm::OccupancyGridTextureMap map(
      c_x, c_y, _distrib_yaw(_gen), 100, 100, &_map_texture_buffer[0], 8 * 8);

    map_merger.merge_map(map);
  }

  std::cout << "benchmark: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::system_clock::now() - ts)
                 .count()
            << "ms" << std::endl;

  map_merger.save("output.pgm");

  std::cout << "Computed with OpenGL " << glGetString(GL_VERSION) << std::endl;

  std::cout << "done." << std::endl;

  return EXIT_SUCCESS;
}