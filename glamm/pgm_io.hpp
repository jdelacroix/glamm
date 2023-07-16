#pragma once

// #include <epoxy/gl.h>
#include <GLES3/gl31.h>

#include <string>

namespace glamm {

void
load_map_from_pgm(const std::string& pgm_url,
                  GLfloat* texture_buffer,
                  const size_t texture_buffer_size);

void
save_map_to_pgm(const std::string& pgm_url,
                const size_t width,
                const size_t height,
                GLubyte* texture_buffer,
                const size_t texture_buffer_size);

}