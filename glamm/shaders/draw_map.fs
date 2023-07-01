#version 320 es

uniform mediump vec3 input_color;

in mediump vec2 target_coordinates;

out mediump vec4 frag_color;

uniform sampler2D input_texture;

void
main()
{
  frag_color = vec4(texture(input_texture, target_coordinates).rgb, 1.0);
}