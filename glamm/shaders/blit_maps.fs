#version 320 es

out mediump vec4 fragment_color;

in mediump vec2 target_coordinates;

uniform sampler2D input_texture;
uniform sampler2D output_texture;

void
main()
{
  mediump vec4 map_color = texture(input_texture, target_coordinates);
  mediump vec4 merged_map_color = texture(output_texture, target_coordinates);

  // if (map_color.a == 1.0) {
  //   if (merged_map_color.a == 1.0) {
  //     fragment_color = vec4(0.31, 0.78, 0.47, 1.0);
  //   } else {
  //     fragment_color = map_color;
  //   }
  // }

  if (map_color.r != 0.5) {
    //   // fragment_color = vec4(0.0, 0.48, 0.65, 1.0);
    fragment_color = vec4(map_color.rgb, 1.0);
  } else {
    fragment_color = vec4(merged_map_color.rgb, 1.0);
  }
}