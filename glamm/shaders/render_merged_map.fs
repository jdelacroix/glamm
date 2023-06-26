#version 320 es

out mediump vec4 fragment_color;

in mediump vec2 target_coordinates;

uniform sampler2D screen_texture;

void
main()
{
  fragment_color = texture(screen_texture, target_coordinates);
}