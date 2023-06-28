#version 320 es

uniform mediump vec3 input_color;

out mediump vec4 frag_color;

void
main()
{
  frag_color = vec4(1.00, 0.46, 0.09, 1.0);
  // frag_color = vec4(input_color, 1.0);
}