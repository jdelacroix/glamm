#version 320 es

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texture;

out vec2 input_position;
out vec2 output_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void
main()
{
  gl_Position = proj * view * model * vec4(vertex.xyz, 1.0);

  input_position = texture;
  output_position =
    vec2((gl_Position.x + 1.0) / 2.0, (gl_Position.y + 1.0) / 2.0);
}