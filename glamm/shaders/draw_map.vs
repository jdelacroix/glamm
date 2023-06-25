#version 320 es

layout(location = 0) in vec2 vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void
main()
{
  gl_Position = proj * view * model * vec4(vertex.xy, 0.0f, 1.0f);
}