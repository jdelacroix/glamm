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

  size_t idx = 0;
  while (buffer >> value) {
    texture_buffer[idx] =
      static_cast<GLfloat>(value) / static_cast<GLfloat>(max_value);
    ++idx;
  }
  std::cout << "(size) = (" << idx << ")" << std::endl;

  for (size_t j = 0; j < height; ++j) {
    for (size_t i = 0; i < width; ++i) {
      std::cout << std::fixed << std::setprecision(1) << std::setw(2)
                << texture_buffer[j * width + i] << " ";
    }
    std::cout << std::endl;
  }
}

}