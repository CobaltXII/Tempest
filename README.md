# Tempest

A terrain editor written in C++11 using OpenGL.

![Alt text](https://github.com/CobaltXII/Tempest/blob/master/screenshot_1.png?raw=true)

# Abstract

Tempest is a terrain editor written in C++ using OpenGL. It was created mainly as an exercise to help me learn some more advanced OpenGL, such as reflections, refractions and other non-trivial effects. Currently, Tempest implements an object-oriented framework with many wrapper classes, such as cameras, cubemaps, displays, entities, framebuffers, heightmaps, lights, models, shaders, terrain, textures and water. Some of the effects that were practised when writing Tempest are water reflections, refractions, normal mapping and depth blending, terrain multi-texturing based on slope and elevation, and terrain decals using signed distance functions.

# Building

Tempest can be compiled and run by first running the library build script and then running the general build script.

```
cobalt$ chmod +x BuildLibraries.sh
cobalt$ ./BuildLibraries.sh
Building glad
Building stb_image
Building stb_image_write
Building tiny_obj_loader
Building FastNoise
Building ImGui
cobalt$ chmod +x BuildTempest.sh
cobalt$ ./BuildTempest.sh
```

# Libraries

Tempest uses [GLAD](http://glad.dav1d.de/) for generating OpenGL loaders, [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) for loading images, [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h) for saving images, [tinyobjloader](https://github.com/syoyo/tinyobjloader) for loading Wavefront models, [FastNoise](https://github.com/Auburns/FastNoise) for generating coherent noise, and [Dear ImGui](https://github.com/ocornut/imgui) for GUI. All of these libraries are under their own respective licenses and are the property of their owners.

# Usage

Just run it. Use W to go forwards, S to go backwards, A to strafe left, D to strafe right, Z to descend, X to ascend, and hold left shift to make any of these actions faster. Hold down the middle mouse button and drag the mouse to look around. When editing, use the scroll wheel to change the size of the cursor, and left- or right-click to apply the selected brush. Press I to toggle the GUI.

# Credits

Thanks to David Herberth, Sean Barrett, Syoyo Fujita, Jordan Peck and Omar Cornut for their libraries. Further thanks to ThinMatrix for his OpenGL tutorials which were great for starting off.

# License

This repository and it's contents are licensed under the MIT License. Any libraries included in this repository are under their own licenses, which can probably be found in their subdirectory.