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

#include "blit_maps_shader.hpp"

#include "shader_program.hpp"

#include <epoxy/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glamm {

BlitMapsShader::BlitMapsShader(const size_t world_width,
                               const size_t world_height)
  : view_(1.0f)
{
  this->shader_id_ = glamm::create_shader_program_from("shaders/blit_maps.vs",
                                                       "shaders/blit_maps.fs");

  float left = -static_cast<float>(world_width) / 2.0f;
  float right = static_cast<float>(world_width) / 2.0f;
  float bottom = -static_cast<float>(world_height) / 2.0f;
  float top = static_cast<float>(world_height) / 2.0f;

  this->proj_ = glm::ortho(left, right, bottom, top, 0.0f, 1.0f);
}

void
BlitMapsShader::draw(const glamm::OccupancyGridTextureMap& map,
                     const unsigned int texture_id,
                     const unsigned int global_texture_id) const
{

  glUseProgram(this->shader_id_);

  glm::mat4 model = map.model();
  glUniformMatrix4fv(glGetUniformLocation(this->shader_id_, "model"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(model));

  glUniformMatrix4fv(glGetUniformLocation(this->shader_id_, "view"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(this->view_));

  glUniformMatrix4fv(glGetUniformLocation(this->shader_id_, "proj"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(this->proj_));

  glUniform1i(glGetUniformLocation(this->shader_id_, "map_texture"), 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, global_texture_id);

  glUniform1i(glGetUniformLocation(this->shader_id_, "input_texture"), 0);
  glUniform1i(glGetUniformLocation(this->shader_id_, "output_texture"), 1);

  map.draw();
}

unsigned int
BlitMapsShader::id() const
{
  return this->shader_id_;
}

}