#version 320 es

uniform mediump vec3 input_color;

in mediump vec2 target_coordinates;

out mediump vec4 frag_color;

uniform sampler2D input_texture;

void
main()
{
  frag_color = vec4(texture(input_texture, target_coordinates).rgb, 1.0);

  // if (target_coordinates.x < 0.5 && target_coordinates.y < 0.5) {
  //   frag_color = vec4(1.00, 0.46, 0.09, 1.0);
  // } else {
  //   frag_color = vec4(0.31, 0.78, 0.47, 1.0);
  // }
}