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

#pragma once

#include <GLES3/gl32.h>
#include <glm/glm.hpp>

#include <array>
#include <cstddef>

namespace glamm {

class OccupancyGridTextureMap
{

public:
  OccupancyGridTextureMap(const float position_x,
                          const float position_y,
                          const float orientation_z,
                          const float width,
                          const float height,
                          const GLfloat* texture_buffer,
                          const size_t texture_width,
                          const size_t texture_height);

  virtual ~OccupancyGridTextureMap();

  void draw() const;
  unsigned int texture_id() const;
  glm::mat4 model() const;

private:
  glm::mat4 model_;
  unsigned int vao_, vbo_, ebo_;
  unsigned int texture_id_;
};

}