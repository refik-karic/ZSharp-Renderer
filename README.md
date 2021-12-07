# ZSharp-Renderer
An old school Win32 3D software renderer implemented as part of a learning experience in computer graphics.

This contains the Windows implementation. Most of the interesting code is in the ZSharp submodule.
https://github.com/refik-karic/ZSharp.git

## In Action
### 34K Vert Low Poly Backpack
![backpack](https://user-images.githubusercontent.com/54491280/131208500-f586aabc-f045-4adb-be9b-4c79d6b2e331.png)
Model Credit to: https://sketchfab.com/3d-models/survival-guitar-backpack-low-poly-799f8c4511f84fab8c3f12887f7e6b36

### Flat Shaded Pyramids
![flat](https://user-images.githubusercontent.com/54491280/63645593-19ceef00-c6d0-11e9-97da-6d78acceb96a.png)

### Wireframe Pyramids
![wire](https://user-images.githubusercontent.com/54491280/63645597-1fc4d000-c6d0-11e9-82af-1c1d465523b5.png)

## Supported Features
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
  - Can register and respond to user input in a platform independent manner. Still requires OS/Device specific layer to feed the input system.

## Requirements to run
- Windows
- Visual Studio 2019
- x86_64 processor

## Features left to be desired
- Texture mapping
- Lighting

## Credits
- Michael Abrash
  - Publicly releasing the Graphics Programming Black Book for free
  - Recommending Computer Graphics Principles and Practice
- David Cherney, Tom Denton, Rohit Thomas and Andrew Waldron (UC Davis Math Dept)
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

## Materials Referenced
- Michael Abrash's Graphics Programming Black Book (ISBN: 1576101746, 1997)
  - https://github.com/jagregory/abrash-black-book
- Computer Graphics: Principles and Practice 2nd Edition (ISBN: 9780201848403, 1995)
- Computer Graphics: Principles and Practice 3rd Edition (ISBN: 0321399528, 2013)
- Expert C Programming: Deep C Secrets (ISBN: 0131774298, 1994)
- Effective C++: 3rd Edition (ISBN: 0321334876, 2005)
- Game Engine Black Book: Wolfenstein 3D 1st Edition (ASIN: B0768B3PWV, 2017)
