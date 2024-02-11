# Trazel Engine - The Second Version of the Trazel 2D Game Engine

## Overview
Trazel Engine is a simple game engine designed for 2D graphics rendering. Its main features include displaying basic shapes such as lines, rectangles, triangles, and dots, as well as applying transformations to these shapes such as rotations, scaling, moving, and more. These transformations can be implemented through the client's C++ code, which can access the engine itself by inheriting the `tze::App` class and overriding its functions.

## Installation & Running

> [!Note]
The project was developed and tested on Windows using an NVIDIA RTX 2060 Super GPU. While it should work on other NVIDIA GPUs and Ryzen CPUs, complete compatibility with other hardware configurations, including Intel CPUs, is not guaranteed.

1. Clone the project.
2. Run the `create_sln.bat` script to create a solution that will work on your PC.
3. Open the solution in Visual Studio and click on the run button to build and execute the project.

## Project Configurations
The project supports three configurations:

### Debug
- Low optimizations.
- Displays all logs on a separate command prompt.

### Release
- High optimization.
- Displays all logs on a separate command prompt, except for special Vulkan logger logs, which are only displayed in Debug mode.

### Client
- High optimization.
- Does not display any logs, only renders the screen to show the output of the shapes.

## Current Goals
- [x] Show a screen.
- [x] Create all mandatory Vulkan components.
- [x] Create simple shapes: Triangles, Lines, Rectangles, Dots, etc.
- [x] Implement simple transformations.
- [ ] Render photos.
- [ ] Add the ability to display textures on shapes.

## How to Use the Engine Inside the Client Solution
> [!Note]
Inside the Client solution, there is a simple example.

1. Create a class that inherits from the `tze::App` class.
2. Implement a constructor and a destructor to create and delete objects as needed.
3. Inside the constructor, create a `tze::GameObject` object and set its properties, such as `vertices`, `model`, `colors`, `transformation component`, and `spin`. Then, use the following function to add the object to the renderer: `_renderer.addGameObjects(object);`