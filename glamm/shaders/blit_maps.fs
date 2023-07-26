#version 320 es

in mediump vec2 input_position;
in mediump vec2 output_position;

out mediump vec4 fragment_color;

uniform sampler2D input_texture;
uniform sampler2D output_texture;

void
main()
{
  mediump float map_value = texture(input_texture, input_position).r;
  mediump vec4 map_color = vec4(map_value, map_value, map_value, 1.0);
  mediump vec4 merged_map_color = texture(output_texture, output_position);

  fragment_color = merged_map_color;

  // if (map_color.r != 0.5 && merged_map_color.a == 1.0) {
  //   fragment_color = vec4(0.31, 0.78, 0.45, 1.0);
  //   return;
  // }

  if (map_color.r != 0.5) {
    if (map_color.r == 0.0 || merged_map_color.r == 0.5) {
      fragment_color = map_color;
    }
  }
}