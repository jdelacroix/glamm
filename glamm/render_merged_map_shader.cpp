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

  float vertices[] = { -1.0f, 1.0f,  0.0f, 1.0f, //
                       -1.0f, -1.0f, 0.0f, 0.0f, //
                       1.0f,  -1.0f, 1.0f, 0.0f, //

                       -1.0f, 1.0f,  0.0f, 1.0f, //
                       1.0f,  -1.0f, 1.0f, 0.0f, //
                       1.0f,  1.0f,  1.0f, 1.0f };

  glGenVertexArrays(1, &(this->vao_));
  glGenBuffers(1, &(this->vbo_));

  glBindVertexArray(this->vao_);

  glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void
RenderMergedMapShader::draw(const unsigned int tbo) const
{
  glUseProgram(this->shader_id_);
  glBindVertexArray(this->vao_);
  std::cout << "Rendering framebuffer texture (" << tbo << ")" << std::endl;
  glBindTexture(GL_TEXTURE_2D, tbo);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

}