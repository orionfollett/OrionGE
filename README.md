# OrionGE

### Required Dependencies:
1. glad for OpenGL function definitions
2. glfw for window management
3. glm for optimized vector and matrix math functions

This project is built on Windows and is not cross platform.


#  Installation and Build Instructions (for VS2022):
1. Clone project
2. Open solution file: OrionGE.sln
3. Toggle to release mode in VS2022
4. Build Solution
5. Run the file OrionGE.exe in LearnOpenGL/x64/Release. 

#  Library Structure:

<p>This project is mainly a loosely coupled collection of C++ header files that 
offers solutions to common functionality that games require. </p>

<p>They are not huge files covering every edge case of game design. Instead they are the bare minimum that is relatively easy
to understand if you just read the code, and don't overwhelm you with options. </p>

<p> This is a personal project for me, so that I don't have to rewrite optimized OpenGL code
  every time I want to draw a cube on the screen. </p>
  
#  Main Useful Header Files

##  Graphics.h 
<p>This is the meat of the application. It includes setting up a window, loading textures, drawing rectangles, boxes, and lines.
   It also provides access to a Camera object for 3D movement around a scene.
   Since this header file has all of the window functions, it also provides a user input callback for keyboard and mouse.
</p>

##  Physics.h 
<p>
This library has collision detection and resolution code for AABBs and Rays.
(Resolution is still a work in progress)
</p>

##  Camera.h 
<p>
Should be accessed through the graphics header file but provides a camera to move around the scene.
</p>
