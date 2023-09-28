# ZSharp-Renderer
An old school Win32 3D software rendered game engine.

This contains the Windows implementation. The majority of the code is in the ZSharp submodule.
https://github.com/refik-karic/ZSharp.git

## In Action

### Texture Mapped Triangle
![TextureTriangle](https://user-images.githubusercontent.com/54491280/263252965-a3581a54-16ff-468d-b98b-58b8e75fe9de.png)

### Flat Shaded Perspective Correct OBJ model from Blender
![OBJFlat](https://user-images.githubusercontent.com/54491280/271285283-93276374-9408-4585-9b18-be590e777655.png)

### Wireframe Version
![OBJWireframe](https://user-images.githubusercontent.com/54491280/266781994-77c6b550-82c7-4166-a56d-ea5a1b207c87.png)

### Depth Vizualization
![OBJDepth](https://user-images.githubusercontent.com/54491280/271284082-cac5b1bb-918b-4d88-8119-983f441a995c.png)

Can render the above 50K vert models (including texture mapping) consistently at <4ms (on an 11900K).

### Flat Shaded Perspective Correct Triangle
![flat](https://user-images.githubusercontent.com/54491280/263252971-57e83ee2-a00c-4a7a-b64b-355d6250f324.png)

### Wireframe Perspective Correct Triangle
![wireframe](https://user-images.githubusercontent.com/54491280/263252970-a0f96d3f-15b7-4050-9dbd-e82d70792bb8.png)

## Features
- Perspective correct attribute interpolation
- Wireframe triangles
- Flat shaded triangles
- Movable perspective camera with WASD controls and virtual trackball mouse
- "Front end" backface culling (before vertex shader)
- NDC clipping
  - With fast path AABB in/out pre-pass that saves a significant amount of time (>10ms)
- Depth buffering
- Fast single precision floating point math library with SIMD intrinsics
- ZSharp static library is platform agnostic
- Standalone
  - No third party libraries are required or included
- Lightweight
- Flexible input system
- Flexible asset system
  - Assets are bundled together into a single binary blob
  - Currently supports OBJ, PNG, and MP3 with more in the future
- MP3 audio
- Platform agnostic programmatic UI text

## Requirements to run
- Windows
- Visual Studio 2019+
- x86_64 processor
  - Many assumptions are made to be running on a 64bit processor. May port to 32bit in the future.
- CMake 3.23+

## Future Work
- Lighting
- Physically Based Materials
- Physics
  - Most likely AABB impulse based

## Credits
- Scratchapixel.com
  - For barycentric perspective correct interpolation
  - https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/perspective-correct-interpolation-vertex-attributes.html
- HandmadeHero/Casey Muratori
  - General game architecture. Platform agnostic API.
  - https://www.youtube.com/watch?v=_4vnV2Eng7M
- Michael Abrash
  - Publicly releasing the Graphics Programming Black Book for free
  - Recommending Computer Graphics Principles and Practice
- UC Davis Math Dept
  - Publicly releasing a free linear albegra textbook
  - https://www.math.ucdavis.edu/~linear/
- Joey De Vries
  - Publicly releasing an OpenGL tutorial for reference and hosting personal projects on GitHub
  - https://learnopengl.com/
- Bob Nystrom
  - Publicly releasing a guide for writing your own interpreter
  - https://craftinginterpreters.com/
- Ebook Foundation
  - Offering lots of useful CS/CE material
  - https://github.com/EbookFoundation/free-programming-books/blob/master/free-programming-books.md
- Onur Mutlu (CMU EE/CE Dept)
  - Publicly releasing full video lectures for the 2015 Spring semester on Computer Architecture
  - https://www.youtube.com/playlist?list=PL5PHm2jkkXmi5CxxI7b3JCL1TWybTDtKq
- DigiPen Game Engine Architecture series
  - https://www.youtube.com/user/GameEngineArchitects/videos
  - Sean Middleditch's lecture on custom math libraries was very helpful
    - https://www.youtube.com/watch?v=v4Ubu7BMXXk
- Squirrel Eiserloh
  - Providing a great example of how to create your own fast and lightweight RNG using noise.
  - https://www.youtube.com/watch?v=LWFzPP8ZbdU

## Materials Referenced
- Michael Abrash's Graphics Programming Black Book (ISBN: 1576101746, 1997)
  - https://github.com/jagregory/abrash-black-book
- Computer Graphics: Principles and Practice 2nd Edition (ISBN: 9780201848403, 1995)
- Computer Graphics: Principles and Practice 3rd Edition (ISBN: 0321399528, 2013)
- Expert C Programming: Deep C Secrets (ISBN: 0131774298, 1994)
- Effective C++: 3rd Edition (ISBN: 0321334876, 2005)
- Game Engine Black Book: Wolfenstein 3D 1st Edition (ASIN: B0768B3PWV, 2017)
