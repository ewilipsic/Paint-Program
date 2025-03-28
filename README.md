This is a paint program made by me.<br>
It is a lightweight, cross-platform paint application built using the Raylib graphics library. It provides a user-friendly interface for creating and editing digital artwork, similar to Microsoft Paint.

### Features
- Intuitive drawing tools (pencil, brush, eraser, etc.)
- Color palette with customizable colors
- Basic shape tools (line, rectangle, circle,outline)
- Symmetry Tool
- FIll Bucket
- File operations (new, open, save)

### Project Structure
<pre>
│── src
│     ├── main.c
│     ├── button.c  
│     ├── pixel.c      
│     ├── vector.c    
│     ├── gui.c      
│     ├── things.h   
│     └── include/
│            ├── raylib.h
│            ├── raymath.h
│            ├── libraylib.a
│            └── rlgl.h
│
│── textures/          # Icons for the buttons
│
│── main.exe           # Windows executable
│
└── README.md
</pre>

#### Files-
main.exe : Windows executable<br>
main.c : MainLoop of the application<br>
button.c : Contains all the functions called by buttons when they are pressed<br>
pixel.c : Contains function related to updating the screen and the basic line drawing<br>
vector.c : Implementation of vector data structure and related utilities<br>
gui.c : a gui object that contains locations and initial state for most of the application<br>
things.h : function declarations and struct definitions for most of the program<br>

#### Raylib Files-
libraylib.a<br>
raylib.h<br>
raymath.h<br>
rlgl.h<br>

### How to compile-
     gcc src/main.c src/gui.c src/pixel.c src/vector.c src/button.c -Iinclude src/include/libraylib.a -lopengl32 -lgdi32 -lwinmm -o main.exe
### GUI EXAMPLE
![alt text](https://github.com/ewilipsic/CS_project/blob/main/readme.png?raw=true)
