# OpenGL 3.3 Render Interface
> Version: 1.1.0  
> License: MIT

This is the backend of OpenGL 3.3.

## Sources
`opengl330.hpp`: Header file  
`opengl330.cpp`: Implementation

## Requirements
* A computer support for OpenGL 3.3 and GLSL (>= 1.3)  
* SDL2, SDL2_image  
* GLEW  
* A compiler support for C++11

## Shaders
GLSL 1.3: `glsl130`

## Custom Shader
If you want to write your own shader for this backend, you have to follow these rules below:

1. Input variables are `position_data`, `color_data`, `texcoord_data` and `normal_data`.  
2. Uniforms are `projection`, `view` and `model`.  
3. Fragment output uses `gl_FragColor`.  

## Extensions
All the names of extended functions/classes/enums etc. are started with "ex".

### Image::(default constructor)
```cpp
Image::Image();
```

Construct an invalid image object.

### Image::(move constructor)
```cpp
Image(Image &&rhs);
```

Allow `std::move` on `Image` object.

### Window::(default constructor)
```cpp
Window::Window();
```

Construct an invalid window object.

### Window::(move constructor)
```cpp
Window(Window &&rhs);
```

Allow `std::move` on `Window` object.

### Window::exRaise
```cpp
void exRaise(const EventType &type, EventArgs *args);
```

Raise a event immediately.

@param type The type of the event to be raised.  
@param args Event arguments.

### Window::exIsFullscreen
```cpp
auto exIsFullscreen() const -> bool;
```

Detect whether this window is fullscreened.

@return bool true for fullscreened window, false otherwise.

### Window::exToggleFullscreen
```cpp
void exToggleFullscreen();
```

Toggle fullscreen state.  
If this window is fullscreened, then it will be windowed.  
If this window is windowed, then it will be fullscreened.

### (static) Window::exGetMousePosition
```cpp
static void exGetMousePosition(int &to_x, int &to_y);
```

Get the mouse position relative to the active window.

@param to_x Save the x position to `to_x`  
@param to_x Save the y position to `to_y`

### Texture::(default constructor)
```cpp
Texture::Texture();
```

Construct an invalid texture object.

### Texture::(move constructor)
```cpp
Texture(Texture &&rhs);
```

Allow `std::move` on `Texture` object.

### VertexBuffer::(move constructor)
```cpp
VertexBuffer(VertexBuffer &&rhs);
```

Allow `std::move` on `VertexBuffer` object.

### IndexBuffer::(move constructor)
```cpp
IndexBuffer(IndexBuffer &&rhs);
```

Allow `std::move` on `IndexBuffer` object.

### Shader::(default constructor)
```cpp
Shader::Shader();
```

Construct an invalid shader object.

### Shader::(move constructor)
```cpp
Shader(Shader &&rhs);
```

Allow `std::move` on `Shader` object.

### Shader::exGetLogInfo
```cpp
auto exGetLogInfo() const -> std::string;
```

Get the shader's info log.

@return std::string Info log.

### Shader::exGetShaderType
```cpp
auto exGetShaderType() const -> ShaderType;
```

Get the type of the shader.

@return ShaderType Shader type.

### ShaderProgram::(default constructor)
```cpp
ShaderProgram::ShaderProgram();
```

Construct an invalid shader program object.

### ShaderProgram::(move constructor)
```cpp
ShaderProgram(ShaderProgram &&rhs);
```

Allow `std::move` on `ShaderProgram` object.

### ShaderProgram::exGetLogInfo
```cpp
auto exGetLogInfo() const -> std::string;
```

Get the shader program's info log.

@return std::string Info log.

### exCullFaceMode
```cpp
enum class exCullFaceMode {
    NoCullFace,
    CW = GL_CW,
    CCW = GL_CCW,
};  // enum class CullFaceMode
```

Enumeration for cull face.

@item NoCullFace Do not set cull face.  
@item CW         Set null face, and clockwise is front face.  
@item CCW        Set null face, and counter-clockwise is front face.

### Renderer::(default constructor)
```cpp
Renderer::Renderer();
```

Construct an invalid renderer object.

### Renderer::(move constructor)
```cpp
Renderer(Renderer &&rhs);
```

Allow `std::move` on `Renderer` object.

### Renderer::exSetCullFace
```cpp
void exSetCullFace(const exCullFaceMode &mode);
```

Set cull face mode.

@param mode Cull face mode. See `exCullFaceMode` for more details.
