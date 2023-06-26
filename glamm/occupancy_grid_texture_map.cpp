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

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/extended_min_max.hpp>

#include <iostream>

namespace glamm {

OccupancyGridTextureMap::OccupancyGridTextureMap(const float position_x,
                                                 const float position_y,
                                                 const float orientation_z,
                                                 const float width,
                                                 const float height)
{
  // position is location at center of map

  float vertices[] = {
    width / 2.0f,  height / 2.0f,  0.0f, // 1.0f, 1.0f, // top right
    width / 2.0f,  -height / 2.0f, 0.0f, // 1.0f, 0.0f, // bottom right
    -width / 2.0f, -height / 2.0f, 0.0f, // 0.0f, 0.0f, // bottom left
    -width / 2.0f, height / 2.0f,  0.0f  // 0.0f, 1.0f, // top left
  };

  unsigned int indices[] = { 0, 1, 3, //
                             1, 2, 3 };

  glGenVertexArrays(1, &(this->vao_));
  glGenBuffers(1, &(this->vbo_));
  glGenBuffers(1, &(this->ebo_));

  glBindVertexArray(this->vao_);

  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // glVertexAttribPointer(
  //   1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  // glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // don't unbind ebo
  glBindVertexArray(0);

  // model matrix (local -> world)

  model_ = glm::mat4(1.0f);
  model_ = glm::translate(model_, glm::vec3(position_x, position_y, 0.0f));
  model_ = glm::rotate(model_, orientation_z, glm::vec3(0.0f, 0.0f, 1.0f));
}

OccupancyGridTextureMap::~OccupancyGridTextureMap()
{
  glDeleteVertexArrays(1, &(this->vao_));
  glDeleteBuffers(1, &(this->vbo_));
  glDeleteBuffers(1, &(this->ebo_));
}

void
OccupancyGridTextureMap::draw() const
{
  glBindVertexArray(this->vao_);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // glBindVertexArray(0);
}

glm::mat4
OccupancyGridTextureMap::model() const
{
  return this->model_;
}

}
