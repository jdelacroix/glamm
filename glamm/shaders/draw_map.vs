#version 320 es

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texture;

out vec2 target_coordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void
main()
{
  gl_Position = proj * view * model * vec4(vertex.xyz, 1.0f);
  target_coordinates = texture;
}