# glamm
(Open)GL Accelerated Map Merging (GLAMM)

## Dependencies

- `libgles-dev` OpenGL 3.2 ES for shaders
- `libglm-dev` for math
- `libegl-dev` for X11-less rendering
- `libgbm-dev` for Linux's Generic Buffer Manager

## How does it work?

0. Maps (textures) are merged sequentially to a framebuffer.
1. Vertex shader computes screen coordinates from map properties (location, orientation, world size).
2. Fragment shader merges map with previously merged maps in the framebuffer via the implemented merge policy.
3. (Optional) Merged map in the framebuffer can be saved to file. 

## What do the shaders do?

1. `BlitMapsShader` merges the newest map with the previously merged maps.

## What are the maps?

Currently, maps are stored in the PGM format and loaded from the filesystem as a texture. Each pixel is interpreted as a cell of the occupancy grid map.