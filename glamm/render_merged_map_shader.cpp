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

#include "render_merged_map_shader.hpp"

#include "shader_program.hpp"

#include <GL/glew.h>

#include <iostream>

namespace glamm {

RenderMergedMapShader::RenderMergedMapShader()
{
  this->shader_id_ = glamm::create_shader_program_from(
    "shaders/render_merged_map.vs", "shaders/render_merged_map.fs");

  float vertices[] = { 1.0f,  1.0f,  1.0f, 1.0f, //
                       1.0f,  -1.0f, 1.0f, 0.0f, //
                       -1.0f, -1.0f, 0.0f, 0.0f, //
                       -1.0f, 1.0f,  0.0f, 1.0f };

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

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // don't unbind ebo
  glBindVertexArray(0);
}

void
RenderMergedMapShader::draw(const unsigned int tbo) const
{
  glUseProgram(this->shader_id_);
  glUniform1i(glGetUniformLocation(this->shader_id_, "screen_texture"), 0);

  std::cout << "Rendering framebuffer texture (" << tbo << ")" << std::endl;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tbo);

  glBindVertexArray(this->vao_);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

}