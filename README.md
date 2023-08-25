# ZSharp-Renderer
An old school Win32 3D software renderer implemented as part of a learning experience in computer graphics.

This contains the Windows implementation. Most of the interesting code is in the ZSharp submodule.
https://github.com/refik-karic/ZSharp.git

## In Action

### Texture Mapped Triangle
![TextureTriangle](https://user-images.githubusercontent.com/54491280/263252965-a3581a54-16ff-468d-b98b-58b8e75fe9de.png)

### Flat Shaded Perspective Correct OBJ model from Blender
![OBJFlat](https://user-images.githubusercontent.com/54491280/263252967-f9552bd1-3b26-4c39-868c-07b5f48d56c9.png)

### Wireframe Version
![OBJWireframe](https://user-images.githubusercontent.com/54491280/263252969-cce0f523-1649-4454-81ac-d92a82181969.png)

### Flat Shaded Perspective Correct Triangle
![flat](https://user-images.githubusercontent.com/54491280/263252971-57e83ee2-a00c-4a7a-b64b-355d6250f324.png)

### Wireframe Perspective Correct Triangle
![wireframe](https://user-images.githubusercontent.com/54491280/263252970-a0f96d3f-15b7-4050-9dbd-e82d70792bb8.png)

## Supported Features
- Perspective correct attribute interpolation
  - RGB color, UV texture
- Wireframe triangles
- Flat shaded triangles
- Movable perspective camera with WASD controls and virtual trackball mouse
- "Front end" backface culling (before vertex shader)
- NDC clipping
- Fast single precision floating point math library with SIMD intrinsics
- ZSharp (static library) is portable
- Standalone
  - No third party libraries are required or included
- Lightweight
  - Compiles to about 417KB (small enough to fit on a floppy disk) in release mode and uses about 9MB of memory for a 1920x1080 viewport

![renderer](https://user-images.githubusercontent.com/54491280/144987286-9ffd78e6-f413-488a-b1b2-f98fded9ec2b.png)
![library](https://user-images.githubusercontent.com/54491280/144987288-a9153e38-7192-4ef9-9b3e-3402ff311d0f.png)
- Flexible input system

## Requirements to run
- Windows
- Visual Studio 2019+
- x86_64 processor
  - Many assumptions are made to be running on a 64bit processor. May port to 32bit in the future.
- CMake 3.23+

## Features left to be desired
- Lighting
- Physically Based Materials

## Credits
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
