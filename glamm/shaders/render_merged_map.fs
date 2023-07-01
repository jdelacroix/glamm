#version 320 es

out mediump vec4 fragment_color;

in mediump vec2 target_coordinates;

uniform sampler2D screen_texture;

void
main()
{
  // mediump vec3 color = texture(screen_texture, target_coordinates).rgb;
  // fragment_color = vec4(color, 1.0);
  fragment_color = texture(screen_texture, target_coordinates);
}