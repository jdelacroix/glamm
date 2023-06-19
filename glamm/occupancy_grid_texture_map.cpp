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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/extended_min_max.hpp>

namespace glamm {

OccupancyGridTextureMap::OccupancyGridTextureMap(const float position_x,
                                                 const float position_y,
                                                 const float orientation_z,
                                                 const size_t width,
                                                 const size_t height)
{
  // viewport should be calculated as the AABB

  glm::mat4 q = glm::mat4(1.0f);
  q = glm::translate(q, glm::vec3(position_x, position_y, 0.0f));
  q = glm::rotate(q, orientation_z, glm::vec3(0.0f, 0.0f, 1.0f));

  glm::vec4 a(0.0f, 0.0f, 0.0f, 1.0f);
  glm::vec4 b(width, 0.0f, 0.0f, 1.0f);
  glm::vec4 c(width, height, 0.0f, 1.0f);
  glm::vec4 d(0.0f, height, 0.0f, 1.0f);

  glm::vec4 aa = q * a;
  glm::vec4 bb = q * b;
  glm::vec4 cc = q * c;
  glm::vec4 dd = q * d;

  auto aabb_max = glm::max(aa, bb, cc, dd);
  auto aabb_min = glm::min(aa, bb, cc, dd);

  viewport_[0] = aabb_min.x;
  viewport_[1] = aabb_min.y;
  viewport_[2] = aabb_max.x;
  viewport_[3] = aabb_max.y;

  // position is location of (0,0) of the map
  // vertices have to be normalized to [-1.0, 1.0] within the viewport
  // default color?
  //
}

}
