# ZSharp-Renderer
An old school Win32 3D software renderer implemented as part of a learning experience in computer graphics

## In Action: Two Pyramids
![flat](https://user-images.githubusercontent.com/54491280/63645593-19ceef00-c6d0-11e9-97da-6d78acceb96a.png)

## Supported Features
- Wireframe triangles
- Flat shaded triangles
- Movable perspective camera
- "Front end" backface culling (before vertex shader)
- NDC clipping
- Templated math library
  - Able to switch between single and double precision floating point "relatively" easily
- ZSharp (static library) is portable
- Standalone
  - No third party libraries are required or included
- Lightweight
  - Compiles to about 6MB in release and uses about 9MB of memory for a 1920x1080 viewport
![resources](https://user-images.githubusercontent.com/54491280/63645734-cc07b600-c6d2-11e9-9c2a-bd26242b9fad.png)

## Requirements to run
- Windows
- Visual Studio 2019
- x86_64 processor

## Known bugs
- Flat shaded scan lines will bleed into each other when two primitives are on the same scan line

## Features left to be desired
- Asset Loader overhaul
  - JSON is parsed everytime as it's loaded, would be more efficient to strip unncessary data
  - Scene path is baked into the renderer (using the one from my machine)
- Texture mapping support
- Lighting support
- Abstract more elements from the main application
  - User inputs

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

## Materials Referenced
- Michael Abrash's Graphics Programming Black Book (ISBN: 1576101746, 1997)
  - https://github.com/jagregory/abrash-black-book
- Computer Graphics: Principles and Practice 2nd Edition (ISBN: 9780201848403, 1995)
- Computer Graphics: Principles and Practice 3rd Edition (ISBN: 0321399528, 2013)
- Expert C Programming: Deep C Secrets (ISBN: 0131774298, 1994)
- Effective C++: 3rd Edition (ISBN: 0321334876, 2005)
- Game Engine Black Book: Wolfenstein 3D 1st Edition (ASIN: B0768B3PWV, 2017)
