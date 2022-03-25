# Model Render

Little toy project.

Based on [learn opengl](https://learnopengl.com/). 

## Features

- **glbase**
  - **obj**: wavefront model file import/render/animator
  - **utils**: shader and texture manage class, other project utilities
  - **thirdparty**: gl environment related library
- **egl**
  - EGL and OpenGL ES environment init in Android platform
- **resource**
  - shader/texture/model, etc. raw files
- **test**
  - single function test
- **main.cpp**
  - a window project in Linux/Windows/MacOS

## Compilation

In pc platform:
```bash
mkdir build && cd build
cmake .. -DGEN_EXE=on
```