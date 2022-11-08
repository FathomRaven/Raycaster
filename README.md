# Raycaster
***
Textured raycaster made in C++ with SDL2, akin to Wolfenstein 3D.

## Features
***
* Textured walls
* Textured floors and ceilings
* Billboard sprite rendering
* External texture and map loading

## Controls
***
`W`, `A`, `S`, and `D` to move around <br><br>
`Left arrow` and `Right arrow` to look around <br><br>
`Up arrow` to load the next map <br><br>
`Down arrow` to load the last map <br><br>

## Credits
***

#### Teaching me about raycasters

* [lodev](https://lodev.org/cgtutor/raycasting.html)

#### Textures are from [opengameart](https://opengameart.org)

* [metal_ceiling.png (4-light-grey-metal-plates)](https://opengameart.org/content/4-light-grey-metal-plates) by Georges "TRaK" Grondin <br>
* [metal.png (rusty metal floor)](https://opengameart.org/content/rusty-metal-floor-texture) by Sindwiller <br>
* [bone_wall.png & bone_wall_pillar.png](https://opengameart.org/content/skull-wall) by p0ss <br>
* [bricks.png](https://opengameart.org/content/bricks-tiled-texture-64x64) by alpha_rats<br>
* [stonewall.png](https://opengameart.org/node/8041) by West:

## Requirements
***
This project requires:

* SDL2

## Compiling
***
Simply compile with your compiler as choice, in this case clang, with the following parameters:

```
clang++ src/*.cpp -I./include/ -lSDL2main -lSDL2 -o main
```

Makefile may or may not work depending on your setup

## Running
***
Run the executable from compilation, either by double clicking it or running:

```
./main
```