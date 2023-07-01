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

  fragment_color = merged_map_color;

  if (map_color.r != 0.5) {
    if (map_color.r == 0.0 || merged_map_color.r == 0.5) {
      fragment_color = map_color;
    }
  }
}