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

#include "map_merger.hpp"

#include "pgm_io.hpp"

#include <GLES3/gl32.h>

namespace glamm {

MapMerger::MapMerger(const size_t width, const size_t height)
  : frame_buffer_(width, height)
  , shader_(width, height)
  , width_(width)
  , height_(height)
{
  this->frame_buffer_.activate();
  glViewport(0, 0, width, height);

  glClearColor(0.5f, 0.5f, 0.5f, 0.0f); // gray
  glClear(GL_COLOR_BUFFER_BIT);
}

void
MapMerger::merge_map(const OccupancyGridTextureMap& map)
{
  this->shader_.draw(map, map.texture_id(), this->frame_buffer_.texture_id());
}

void
MapMerger::save(const std::string& pgm_url) const
{
  GLubyte output_buffer[this->width_ * this->height_];
  for (size_t i = 0; i < this->width_ * this->height_; ++i) {
    output_buffer[i] = 0; // initialize to black
  }

  glReadPixels(0,
               0,
               this->width_,
               this->height_,
               GL_RED,
               GL_UNSIGNED_BYTE,
               // _width * _height,
               &output_buffer[0]);

  glamm::save_map_to_pgm(pgm_url,
                         this->width_,
                         this->height_,
                         &output_buffer[0],
                         this->width_ * this->height_);
}

}