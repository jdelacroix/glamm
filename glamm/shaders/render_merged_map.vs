#version 320 es

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 texture;

out vec2 target_coordinates;

void
main()
{
  gl_Position = vec4(vertex.x, vertex.y, 0.0, 1.0);
  target_coordinates = texture;
}
