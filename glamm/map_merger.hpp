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

#include "blit_maps_shader.hpp"
#include "frame_buffer.hpp"
#include "occupancy_grid_texture_map.hpp"

#include <string>

namespace glamm {

class MapMerger
{
public:
  MapMerger(const size_t width, const size_t height);

  void merge_map(const OccupancyGridTextureMap& map);
  void save(const std::string& pgm_url) const;

private:
  FrameBuffer frame_buffer_;
  BlitMapsShader shader_;
  size_t width_, height_;
};

}