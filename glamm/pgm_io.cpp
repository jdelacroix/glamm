#include "pgm_io.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace glamm {

void
load_map_from_pgm(const std::string& pgm_url,
                  GLfloat* texture_buffer,
                  const size_t texture_buffer_size)
{
  std::ifstream ifs(pgm_url, std::ios::binary);

  std::string input_line;
  std::getline(ifs, input_line);
  if (input_line != "P2") {
    throw std::runtime_error("Incorrect PGM format. It must be P2.");
  }

  while (ifs.peek() == '#') {
    std::getline(ifs, input_line);
  } // discard comments

  std::stringstream buffer;

  buffer << ifs.rdbuf();

  unsigned int width = 0, height = 0;

  int value = 0, max_value = 0;

  buffer >> width >> height;
  std::cout << "(width, height) = (" << width << ", " << height << ")"
            << std::endl;
  buffer >> max_value;
  std::cout << "(max_value) = (" << max_value << ")" << std::endl;

  // float float_value = 0.0f;

  // for (size_t i = 0; i < (width * height); ++i) {
  //   buffer >> value;

  //   // float_value = static_cast<float>(value) /
  //   static_cast<float>(max_value);

  //   texture_buffer[3 * i] = value;
  //   texture_buffer[3 * i + 1] = value;
  //   texture_buffer[3 * i + 2] = value;
  // }

  std::vector<int> texture;
  texture.reserve(width * height);
  while (buffer >> value) {
    texture.push_back(value);
  }
  std::cout << "(size) = (" << texture.size() << ")" << std::endl;

  for (size_t j = 0; j < height; ++j) {
    for (size_t i = 0; i < width; ++i) {
      std::cout << std::setw(2) << texture[j * width + i] << " ";
    }
    std::cout << std::endl;
  }

  GLfloat value_f;

  for (size_t i = 0; i < texture.size(); ++i) {

    value_f =
      static_cast<GLfloat>(texture[i]) / static_cast<GLfloat>(max_value);
    std::cout << std::fixed << std::setprecision(2) << std::setw(3) << value_f
              << " ";
    texture_buffer[3 * i] = value_f;
    texture_buffer[3 * i + 1] = value_f;
    texture_buffer[3 * i + 2] = value_f;

    if (i % width == (width - 1)) {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}

}