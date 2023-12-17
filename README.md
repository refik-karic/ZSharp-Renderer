# ZSharp-Renderer
An old school Win32 3D software rendered game engine.

This contains the Windows implementation. The majority of the code is in the ZSharp submodule.
https://github.com/refik-karic/ZSharp.git

## In Action

### Physics
![Physics](https://user-images.githubusercontent.com/54491280/282179157-1da80882-14f8-4893-8a60-565dae40de2f.gif)

### Texture Mapped Triangle
![TextureTriangle](https://user-images.githubusercontent.com/54491280/263252965-a3581a54-16ff-468d-b98b-58b8e75fe9de.png)

### UV Flat Shaded Perspective Correct OBJ model from Blender
![OBJFlatUV](https://user-images.githubusercontent.com/54491280/271782724-29378fd3-5d3b-4564-a0ec-c1ed34067fe5.png)

### RGB Flat Shaded Perspective Correct OBJ model from Blender
![OBJFlatRGB](https://user-images.githubusercontent.com/54491280/271782726-cbed0fd5-fe08-4f70-aec5-2d0c9a64291d.png)

### Wireframe Version
![OBJWireframe](https://user-images.githubusercontent.com/54491280/266781994-77c6b550-82c7-4166-a56d-ea5a1b207c87.png)

### Depth Vizualization
![OBJDepth](https://private-user-images.githubusercontent.com/54491280/291082941-cbfe9584-1615-4428-8ed0-117bc0e2996c.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTEiLCJleHAiOjE3MDI4MzA4OTcsIm5iZiI6MTcwMjgzMDU5NywicGF0aCI6Ii81NDQ5MTI4MC8yOTEwODI5NDEtY2JmZTk1ODQtMTYxNS00NDI4LThlZDAtMTE3YmMwZTI5OTZjLnBuZz9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFJV05KWUFYNENTVkVINTNBJTJGMjAyMzEyMTclMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjMxMjE3VDE2Mjk1N1omWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPWMxYjcxMjJkMTU0Njk1MGQ4ZjMyMmNiOTJmOTZjNWRhYTMyZjNhMDc2OGE3ZGQwN2Y4MGM5ZThhY2M0OGRhZmYmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0JmFjdG9yX2lkPTAma2V5X2lkPTAmcmVwb19pZD0wIn0._8OGU94TmCgauH9WDkB7zWejuq5vGx-Pa2conip_ueE)

Can render the above 50K vert models (including texture mapping) consistently at <3ms (on an 11900K).

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
- Continuous collision detection physics sytem
  - Currently only supports Static vs Dynamic collisions
  - Does not have a BSP/AABB tree partition yet, it currently just loops over all objects regardless of distance
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

## Materials Referenced
- Michael Abrash's Graphics Programming Black Book (ISBN: 1576101746, 1997)
  - https://github.com/jagregory/abrash-black-book
- Computer Graphics: Principles and Practice 2nd Edition (ISBN: 9780201848403, 1995)
- Computer Graphics: Principles and Practice 3rd Edition (ISBN: 0321399528, 2013)
- Expert C Programming: Deep C Secrets (ISBN: 0131774298, 1994)
- Effective C++: 3rd Edition (ISBN: 0321334876, 2005)
- Game Engine Black Book: Wolfenstein 3D 1st Edition (ASIN: B0768B3PWV, 2017)
- Real-Time Collision Detection (ISBN: 1558607323, 2004)
