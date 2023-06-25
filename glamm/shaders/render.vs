#version 320 es

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coordinates;

out vec2 target_coordinates;

void
main()
{
  gl_Position = vec4(vertex_position.x, vertex_position.y, 0.0, 1.0);
  target_coordinates = texture_coordinates;
}
