# glamm
OpenGL Accelerated Map Merging (GLAMM)

## Dependencies

- `freeglut-dev` for OpenGL accelerated visualization
- `libglew-dev` for loading OpenGL (extensions) > 1.0
- `libglm-dev` for math
- OpenGL 3.2 ES

## How should it work?

0. Maps (textures) are merged sequentially to two framebuffer.
1. The first framebuffer is an empty map (transparent texture) from which the shader will read and combine with a new piece of the map (a rectangle with a bound texture loaded from a PGM) to render to the second framebuffer.
2. The last-to-be-written framebuffer contains the merged map and can be written to the filesystem as a PGM. 
3. (Optional) Display the merged map on the screen.

## What do the shaders do?

1. `DrawMapShader` draws a new map to a buffer.
2. `BlitMapsShader` merges the newest map with the previously merged maps.
3. `RenderMergedMapShader` draw the merged map to the screen.