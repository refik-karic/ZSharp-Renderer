# ZSharp-Renderer
An old school Win32 3D software rendered game engine.

This contains the Windows implementation. The majority of the code is in the ZSharp submodule.
https://github.com/refik-karic/ZSharp.git

## In Action

### Demo

https://github.com/user-attachments/assets/46d612ac-333f-4a98-a9c8-dffcbc96f4ae

## Features
- Perspective correct attribute interpolation
- Wireframe drawing
- RGB/Texture mapped drawing
- Movable perspective camera with WASD controls and virtual trackball mouse
- Backface culling
- NDC clipping
  - With fast path AABB in/out pre-pass that saves a significant amount of time (>10ms)
- Depth buffering
- Mipmapping
- Fast single precision floating point math library with SIMD intrinsics
- ZSharp static library is platform agnostic
- Standalone
  - No third party libraries are required or included
    - In some cases this isn't possible due to standards not being publicly available (i.e. ISO spec that costs hundreds of dollars in order to obtain a copy). To workaround this, I've made a best effort to implement the spec with as much freely available information there is on the web. In some cases, such as with MP3 or JPEG, I've decided to rely on Public Domain or Creative Commons code to supplement my work or to provide the entire feature. 
     Again, this is only in extreme cases where I've had to workaround the paywall. I am in no way claiming those works are entirely my creation. I have made a best effort to credit the Authors below.
- Lightweight
- Simple programmer driven UI system
- Flexible input system
- Flexible asset system
  - Assets are bundled together into a single binary blob
  - Currently supports OBJ, PNG, and MP3 with more in the future
- MP3 audio
- Continuous collision detection physics sytem
  - Currently only supports Static vs Dynamic collisions
  - Does not have a BSP/AABB tree partition yet, it currently just loops over all objects regardless of distance
- Platform agnostic programmatic UI text
- Developer console for toggling various things in the engine

## Requirements to run
- Windows
- Visual Studio 2019+
- x86_64 processor
  - Many assumptions are made to be running on a 64bit processor. May port to 32bit in the future.
- CMake 3.23+

## Future Work
- Lighting
- Physically Based Materials

## Materials Referenced
### Books
- Michael Abrash's Graphics Programming Black Book (ISBN: 1576101746, 1997)
  - https://github.com/jagregory/abrash-black-book
- Computer Graphics: Principles and Practice 2nd Edition (ISBN: 9780201848403, 1995)
- Computer Graphics: Principles and Practice 3rd Edition (ISBN: 0321399528, 2013)
- Expert C Programming: Deep C Secrets (ISBN: 0131774298, 1994)
- Effective C++: 3rd Edition (ISBN: 0321334876, 2005)
- Game Engine Black Book: Wolfenstein 3D 1st Edition (ASIN: B0768B3PWV, 2017)
- Real-Time Collision Detection (ISBN: 1558607323, 2004)

### Web
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
  - Intro to Game Physics was very helpful
    - https://www.youtube.com/watch?v=wPKzwSxyhTI
- Squirrel Eiserloh
  - Providing a great example of how to create your own fast and lightweight RNG using noise.
  - https://www.youtube.com/watch?v=LWFzPP8ZbdU
- Christer Ericson
  - Real Time Collision Detection was immensely useful in understanding and applying physics
- Erin Catto
  - GDC talks were very helpful in understanding what goes into a real time physics system
  - https://youtu.be/7_nKOET6zwI?feature=shared
- Fabian Giesen
  - Demonstrating an efficent way to step the inner raster loop just by adding some offsets
  - https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/

### Public Domain/Creative Commons Code
- Rich Geldreich
  - JPEG Decoder: https://github.com/richgel999/jpeg-compressor
- lieff: https://github.com/lieff
  - MP3 Decoder: https://github.com/lieff/minimp3
- Marcel Sondaar
  - IBM public domain VGA fonts: https://github.com/dhepper/font8x8
