
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

#include <string>

namespace glamm {

// class ShaderProgram
// {
// public:
//   ShaderProgram(const std::string& vertex_shader_url,
//                 const std::string& framgent_shader_url);

//   unsigned int id() const;

// private:
//   unsigned int id_;
// };

unsigned int
create_shader_program_from(const std::string& vertex_shader_url,
                           const std::string& fragment_shader_url);

}