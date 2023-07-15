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

#include "frame_buffer.hpp"

#include <GL/glew.h>

#include <stdexcept>

namespace glamm {

FrameBuffer::FrameBuffer(const unsigned int width, const unsigned int height)
{
  glGenFramebuffers(1, &(this->id_));
  glBindFramebuffer(GL_FRAMEBUFFER, this->id_);

  glGenTextures(1, &(this->texture_id_));
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->texture_id_);

  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture_id_, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Framebuffer is not complete!");
  }

  // glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind to avoid accidental rendering
}

void
FrameBuffer::activate() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->id_);
  // glClearColor(0.31f, 0.78f, 0.47f, 1.0f); // emerald green
  // glClear(GL_COLOR_BUFFER_BIT);
  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, this->texture_id_);
}

unsigned int
FrameBuffer::texture_id() const
{
  return this->texture_id_;
}

}