
#pragma once

#include <string>

namespace glamm {

class ShaderProgram
{
public:
  ShaderProgram(const std::string& vertex_shader_url,
                const std::string& framgent_shader_url);

  unsigned int id() const;

private:
  unsigned int id_;
};
}