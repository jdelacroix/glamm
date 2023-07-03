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

#include "occupancy_grid_texture_map.hpp"

#include <glm/glm.hpp>

namespace glamm {

class DrawMapShader
{
public:
  DrawMapShader(const size_t world_width, const size_t world_height);

  void draw(const glamm::OccupancyGridTextureMap& map,
            const unsigned int texture_id,
            const unsigned int global_texture_id) const;

  unsigned int id() const;

private:
  unsigned int shader_id_;
  glm::mat4 view_, proj_;
  float world_width_, world_height_;
};

}