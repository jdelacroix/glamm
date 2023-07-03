#version 320 es

in mediump vec2 input_position;
in mediump vec2 output_position;

out mediump vec4 fragment_color;

uniform sampler2D input_texture;
uniform sampler2D output_texture;

void
main()
{
  mediump vec4 map_color = texture(input_texture, input_position);
  mediump vec4 merged_map_color = texture(output_texture, output_position);

  fragment_color = merged_map_color;

  if (map_color.r != 0.5) {
    if (map_color.r == 0.0 || merged_map_color.r == 0.5) {
      fragment_color = map_color;
    }
  }
}