# glamm
(Open)GL Accelerated Map Merging (GLAMM)

## Dependencies

- OpenGL 3.2 ES for shaders
- `libepoxy-dev` for loading OpenGL (extensions) > 1.0
- `libglm-dev` for math

### Headless

- `libgbm-dev` for Linux's Generic Buffer Manager
- `libegl-dev` for X11-less rendering
- `libegl-mesa0` for the Mesa implementation of EGL

### Visualization

- `freeglut-dev` for OpenGL accelerated visualization

## How does it work?

0. Maps (textures) are merged sequentially to a framebuffer.
1. Vertex shader computes screen coordinates from map properties (location, orientation, world size).
2. Fragment shader merges map with previously merged maps in the framebuffer via the implemented merge policy.
3. (Optional) Merged map in the framebuffer can be rendered to screen. 

## What do the shaders do?

2. `BlitMapsShader` merges the newest map with the previously merged maps.
3. (Optional) `RenderMergedMapShader` draw the merged map to the screen.

## What are the maps?

Currently, maps are stored in the PGM format and loaded from the filesystem as a texture. Each pixel is interpreted as a cell of the occupancy grid map.