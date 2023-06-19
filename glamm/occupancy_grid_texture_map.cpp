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

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/extended_min_max.hpp>

namespace glamm {

OccupancyGridTextureMap::OccupancyGridTextureMap(const float position_x,
                                                 const float position_y,
                                                 const float orientation_z,
                                                 const size_t width,
                                                 const size_t height)
{
  // position is location at center of map

  vertices_[0] = glm::vec4(-width / 2.0f, -height / 2.0f, 0.0f, 1.0f);
  vertices_[1] = glm::vec4(width / 2.0f, -height / 2.0f, 0.0f, 1.0f);
  vertices_[2] = glm::vec4(width / 2.0f, height / 2.0f, 0.0f, 1.0f);
  vertices_[3] = glm::vec4(-width / 2.0f, height / 2.0f, 0.0f, 1.0f);

  // model matrix (local -> world)

  model_ = glm::mat4(1.0f);
  model_ = glm::translate(model_, glm::vec3(position_x, position_y, 0.0f));
  model_ = glm::rotate(model_, orientation_z, glm::vec3(0.0f, 0.0f, 1.0f));
}

}
