# Fake Agar.io Render Interfaces Specific
> Version: V0.1.32
> License: MIT

## 目标
封装**底层**图形接口。
统一Direct3D和OpenGL的接口。

##使用流程：
```
载入（Initialize） -> 创建窗口 -> 载入着色器 -> 制作着色程序 -> 创建渲染器 -> 绑定着色程序 -> 设置渲染器 -> 「Application Loop」 -> [销毁资源（RAII）] -> 退出（Terminate）
```

## 命名空间
所有的接口统一定义在此命名空间
```cpp
namespace render {

}  // namespace render
```

## 内存管理
使用C++的RAII机制。
除了特殊的资源（如缓冲）使用工厂函数构造外，其它的资源使用**构造函数**创建，使用**析构函数**销毁。

## 特殊识别
使用`BACKEND_*`来表示实现的API。
```
#define BACKEND_UNKNOWN

// DirectX后端
#define BACKEND_DIRECT3D9
#define BACKEND_DIRECT3D10
#define BACKEND_DIRECT3D11
#define BACKEND_DIRECT3D12
#define BACKEND_DIRECT2D

// OpenGL后端
#define BACKEND_OPENGL210
#define BACKEND_OPENGL330
#define BACKEND_OPENGL450
#define BACKEND_OPENGLES
#define BACKEND_OPENGLES3

// Vulkan
#define BACKEND_VULKAN

// Metal
#define BACKEND_METAL

#ifdef BACKEND_DIRECT3D11
// Something special
#endif
```

所有实现的头文件用`render.h`统一管理。
使用`USE_BACKEND_*`来决定使用何种API。
`render.h`文件类容大致如下：
```cpp
// in render.h

#ifndef RENDER_H_
#define RENDER_H_

#ifdef USE_BACKEND_DIRECT3D9
#include "file-to-direct3d9.h"
#endif

#ifdef USE_BACKEND_OPENGL330
#include "file-to-opengl330.h"
#endif

#endif
```

## 加载
如果需要初始化或释放底层接口，通过以下两个函数
```cpp
// 载入底层接口
void Initialize();

// 释放底层接口
void Terminate();
```

## 编码
统一使用UTF-16。
C++的Unicode库可以使用[cpputf8](http://utfcpp.sourceforge.net/)。
C++中使用UTF-16字面值常量使用`u`前缀，如`u"中文"`。
```cpp
typedef char16_t CharType;
typedef const CharType* UTF16String;
```

**注意**：不推荐自定义`CharType`为`wchar_t`,因为在不同的平台上`wchar_t`的大小会有所不同。最好是使用硬性规定好的`char16_t`或`uint16_t`。

## 第三方库
数学库： [glm](https://github.com/g-truc/glm)

```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
```

## 资源
### 图像
图像表示本地的资源，正常情况下是存储在内存中
```cpp
class Image {
 public:
    // 从文件加载
    Image(const UTF16String filepath);
    
    // 销毁
    ~Image();
    
    // 保存到文件
    void Save(const UTF16String filepath);
    
    // 宽度 in pixels
    auto GetWidth() const -> int;
    
    // 高度 in pixels
    auto GetHeight() const -> int;
    
    // 是否有效
    auto IsValid() const -> bool;
};
```

## 平台相关
### 事件处理
`EventArgs`是所有事件数据的基础结构体
```cpp
struct EventArgs {
    // Empty
};
```

事件类型：
```cpp
enum class EventType {
    // 窗口关闭、窗口重置大小、鼠标移动、鼠标滚轮转动、鼠标点击、键盘按键
    Close, Resize, MouseMotion, MouseWheel, MouseClick, Keyboard
};

struct CloseEventArgs {
    // Empty
};

struct ResizeEventArgs {
    // 新的大小
    int width;
    int height;
};

struct MouseMotionEventArgs {
    // 鼠标的位置
    int x;
    int y;
};

struct MouseWheelEventArgs {
    // 鼠标位置
    int x;
    int y;
    
    // 滚轮转动量（offest_x通常为0）
    int offest_x;
    int offest_y;
};

// 鼠标按键可以重新定值，以适配不同的平台
enum class MouseButton {
    // 左键、中建、右键
    Left, Middle, Right  
};

struct MouseClickEventArgs {
    // 鼠标位置
    int x;
    int y;
    
    // 鼠标按键
    MouseButton button;
    
    // 如果按下并未释放，为true
    bool pressed;
    
    // 如果是按下后释放了，为true
    bool released;
}

// 按键的具体值需要重写
// 以下为必须有的按键
enum class Keycode {
    A-Z, 0-9, Numpad0-Numpad-9,
    Quote: "'",
    Backslash: "\",
    Backspace,
    CapsLock,
    Comma: ",",
    Delete,
    Down,
    End,
    Equal: "=",
    Escape: "ESC",
    F1-F24,
    Backquote: "`",
    Home,
    Insert,
    LeftAlt,
    LeftControl,
    Left,
    Leftbracket: "[",
    LeftShift,
    Minus: "-",
    NumLock,
    PageDown,
    PageUp,
    Period: ".",
    PrintScreen,
    RightAlt,
    RightControl,
    RightShift,
    Return: "Enter",
    Right,
    Rightbracket: "]",
    ScrollLock,
    Semicolon: ";",
    Slash: "/",
    Space,
    Tab,
    Unknown,
    Up,
    Ampersand: "&",
    Asterisk: "*",
    At: "@",
    Caret: "^",
    Colon: ":",
    Dollar: "$",
    Exclaim: "!",
    Greater: ">",
    Hash: "#",
    LeftParen: "(",
    Less: "<",
    Percent: "%",
    Plus: "+",
    Question: "?",
    DoubleQuote: """,
    RightParen: ")",
    Underscore: "_"
};

// 枚举值必须为2的幂
enum Modifier : unsigned {
    LCTRL, RCTRL, LSHIFT, RSHIFT, LALT, RALT, NUM_LOCK, SCROLL_LOCK, CAPS_LOCK,
    SHIFT, CTRL, ALT
};

struct KeyboardEventArgs {
    // 按键
    Keycode code;
    
    // 特殊按键（CTRL、SHIFT和ALT这些），每个按键占一个二进制位
    // 通过按位与（&）来确定某个键是否被按下
    // 例如：
    // modifiers & (LCTRL | LSHIFT) == true 表明左Control和左Shift被同时按下
    unsigned modifiers;
    
    // 如果按下并未释放，为true
    bool pressed;
    
    // 如果是按下后释放了，为true
    bool released;
};
```

回调函数：
```cpp
// 第一个参数是发送者，通常为收到事件的窗口
// 第二个参数是事件的数据
typedef std::function<void(void *, EventArgs *)> CallbackType;
```

事件处理：
```cpp
// 在此函数中处理所有窗口的事件
void DoWindowEvents();
```

示例
```
void on_key_press(void *sender, EventArgs *args) {
    Window *window = reinterpret_cast<Window *>(sender);
    KeyboardEventArgs *event = reinterpret_cast<KeyboardEventArgs *>(args);
    
    if (event->code == Keycode::Escape) {
        window->Close();
    }
}
```

### 窗口
```cpp
class Window {
 public:
    Window(
        const int width,             // 窗口宽度
        const int height,            // 窗口高度
        const UTF16String title,     // 标题
        const Image &icon,           // 图标
        const bool fullscreen=false  // 是否全屏
    );
    ~Window();
    
    // 将回调函数绑定到某一事件上
    void AddHandler(const EventType &type, const CallbackType &callback);
    
    // 删除某一事件的**所有**绑定的回调函数
    void RemoveHandlers(const EventType &type);
    
    // 关闭窗口
    // 应确保会引发CloseEvent
    void Close();
    
    // 重置窗口大小
    // 应确保会引发ResizeEvent
    void Resize(const int width, const int height);
    
    auto IsValid() const -> bool;
};
```

事件处理的过程：
所有的事件都是要交给指定的回调函数来处理，使用`AddHandler`来添加回调函数。
同一事件可以有多个回调函数，不保证回调函数调用的顺序。
使用`RemoveHandlers`将删除某一事件**所有**的回调函数。
窗口在运行中可能会随时收到事件消息，但不应立马调用回调函数。使用`DoWindowEvents`函数来集中处理这些事件。

**全屏**：使用**真全屏**

## 渲染
### 材质
材质是指存储在显存上的资源。
在Direct3D中称为“贴图”。
```cpp
class Texture {
 public:
    // 从图像载入
    Texture(const Image &image);
    ~Texture();
    
    auto IsValid() const -> bool;
};
```

### 缓冲
顶点格式：
```cpp
struct Vertex {
    // 空间坐标
    float x, y, z;
    
    //颜色
    float r, g, b, a;
    
    // 贴图坐标
    float u, v;
    
    // 法线
    float nx, ny, nz;
};
```

**所有缓冲通过`Renderer`创建。**
顶点缓冲：
```cpp
class VertexBuffer {
 public:
    ~VertexBuffer();
    
    auto IsValid() const -> bool;
};
```

索引缓冲：
```cpp
class IndexBuffer {
 public:
    ~IndexBuffer();
    
    auto IsValid() const -> bool;
};
```

### 着色器
着色器类型：
```cpp
enum class ShaderType {
    // 顶点着色器，像素着色器
    VertexShader, PixelShader
};
```

着色器：
**注意**：所有着色器的入口函数为`main`。
```cpp
class Shader {
 public:
    // 从文件载入指定类型的着色器
    Shader(const UTF16String filepath, const ShaderType &type);
    ~Shader();
    
    auto IsValid() const -> bool;
};
```

着色程序：
着色程序是顶点着色器和像素着色器的结合体
```cpp
class ShaderProgram {
 public:
    ShaderProgram(Shader *vertex, Shader *pixel);
    ~ShaderProgram();
    
    auto IsValid() const -> bool;
};
```

### 渲染器
渲染器负责掌管渲染状态和渲染。
```cpp
class Renderer {
 public:
    // 创建一个绑定到指定窗口的渲染器
    Renderer(Window *window, ShaderProgram *program);
    ~Renderer();
    
    // 三大矩阵
    void SetProjectionMatrix(const mat4 &matrix);
    void SetModelMatrix(const mat4 &matrix);
    void SetViewMatrix(const mat4 &matrix);
    
    // 设置材质
    void BindTexture(const Texture &texture);
    void UnbindAllTexture();
    
    // 重设着色程序
    // 切换着色程序需要重新设置渲染器
    void ResetShaderProgram(ShaderProgram *program);
    
    // 创建缓冲
    void CreateVertexBuffer(VertexBuffer &target, const int size, Vertex *data);
    void CreateIndexBuffer(IndexBuffer &target, const int size, unsigned *data);

    // 用STL容器来创建缓冲（已实现）
    template <typename TContainer>
    void CreateVertexBuffer(VertexBuffer &target, const TContainer &data) {
        CreateVertexBuffer(target, data.size(), data.data());
    }
    
    template <typename TContainer>
    void CreateIndexBuffer(IndexBuffer &target, const TContainer &data) {
        CreateIndexBuffer(target, data.size(), data.data());
    }
    
    // 清空
    void Clear(
        const float red = 0.0f,
        const float green = 0.0f,
        const float blue = 0.0f
    );
    
    // 开始绘图
    void Begin();
    
    // 结束绘图
    void End();
    
    // 刷新
    void Present();
    
    // 绘制缓冲
    template <typename TBuffer>
    void DrawBuffer(const TBuffer &buffer);
```

实现`DrawBuffer`函数的正确方式是模板特化：
```cpp
// In .cpp file
// 顶点缓冲的特化版本
template <>
void render::Renderer::DrawBuffer(const render::VertexBuffer &buffer);

// 索引缓冲的特化版本
template <>
void render::Renderer::DrawBuffer(const render::IndexBuffer &buffer);
};
```

## 通用接口
以下是通用接口，需要C++11支持。
所有需要实现者实现的部分均用`/*implement this*/`做了标记。
```cpp
//
// Copyright 2016 HNSDFZ-OIER
//

#ifndef RENDER_UNIVERSAL_HPP_
#define RENDER_UNIVERSAL_HPP_

#include <cstdint>
#include <functional>

// thirdparty library glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wheader-hygiene"
using namespace glm;
#pragma clang diagnostic pop
#endif

#define BACKEND_UNKNOWN

namespace render {

typedef char16_t CharType;
typedef const CharType *UTF16String;

void Initialize(); /*implement this*/
void Terminate();  /*implement this*/

///////////////
// Resources //
///////////////

class Image {
 public:
    Image() = delete;
    Image(const UTF16String filepath); /*implement this*/
    ~Image();                          /*implement this*/

    Image(const Image &) = delete;
    auto operator=(const Image &) -> Image & = delete;

    Image(Image &&) = delete;
    auto operator=(Image && ) -> Image & = delete;

    void Save(const UTF16String filepath); /*implement this*/
    auto GetWidth() const -> int;          /*implement this*/
    auto GetHeight() const -> int;         /*implement this*/
    auto IsValid() const -> bool;          /*implement this*/
};                                         // class Image

//////////////
// Platform //
//////////////

void DoWindowEvents(); /*implement this*/

enum class EventType {
    Close,
    Resize,
    MouseMotion,
    MouseWheel,
    MouseClick,
    Keyboard,
};  // enum class EventType

/*implement this*/
enum class MouseButton {
    Left,
    Middle,
    Right,
};  // enum class MouseButton

/*implement this*/
enum class Keycode {
    Unknown = 0,
    A = Unknown,
    B = Unknown,
    C = Unknown,
    D = Unknown,
    E = Unknown,
    F = Unknown,
    G = Unknown,
    H = Unknown,
    I = Unknown,
    J = Unknown,
    K = Unknown,
    L = Unknown,
    M = Unknown,
    N = Unknown,
    O = Unknown,
    P = Unknown,
    Q = Unknown,
    R = Unknown,
    S = Unknown,
    T = Unknown,
    U = Unknown,
    V = Unknown,
    W = Unknown,
    X = Unknown,
    Y = Unknown,
    Z = Unknown,
    Num0 = Unknown,
    Num1 = Unknown,
    Num2 = Unknown,
    Num3 = Unknown,
    Num4 = Unknown,
    Num5 = Unknown,
    Num6 = Unknown,
    Num7 = Unknown,
    Num8 = Unknown,
    Num9 = Unknown,
    Numpad0 = Unknown,
    Numpad1 = Unknown,
    Numpad2 = Unknown,
    Numpad3 = Unknown,
    Numpad4 = Unknown,
    Numpad5 = Unknown,
    Numpad6 = Unknown,
    Numpad7 = Unknown,
    Numpad8 = Unknown,
    Numpad9 = Unknown,
    Quote = Unknown,
    Backslash = Unknown,
    Backspace = Unknown,
    CapsLock = Unknown,
    Comma = Unknown,
    Delete = Unknown,
    Down = Unknown,
    End = Unknown,
    Equal = Unknown,
    Escape = Unknown,
    F1 = Unknown,
    F2 = Unknown,
    F3 = Unknown,
    F4 = Unknown,
    F5 = Unknown,
    F6 = Unknown,
    F7 = Unknown,
    F8 = Unknown,
    F9 = Unknown,
    F10 = Unknown,
    F11 = Unknown,
    F12 = Unknown,
    F13 = Unknown,
    F14 = Unknown,
    F15 = Unknown,
    F16 = Unknown,
    F17 = Unknown,
    F18 = Unknown,
    F19 = Unknown,
    F20 = Unknown,
    F21 = Unknown,
    F22 = Unknown,
    F23 = Unknown,
    F24 = Unknown,
    Backquote = Unknown,
    Home = Unknown,
    Insert = Unknown,
    LeftAlt = Unknown,
    LeftControl = Unknown,
    Left = Unknown,
    Leftbracket = Unknown,
    LeftShift = Unknown,
    Minus = Unknown,
    NumLock = Unknown,
    PageDown = Unknown,
    PageUp = Unknown,
    Period = Unknown,
    PrintScreen = Unknown,
    RightAlt = Unknown,
    RightControl = Unknown,
    RightShift = Unknown,
    Return = Unknown,
    Right = Unknown,
    Rightbracket = Unknown,
    ScrollLock = Unknown,
    Semicolon = Unknown,
    Slash = Unknown,
    Space = Unknown,
    Tab = Unknown,
    Up = Unknown,
    Ampersand = Unknown,
    Asterisk = Unknown,
    At = Unknown,
    Caret = Unknown,
    Colon = Unknown,
    Dollar = Unknown,
    Exclaim = Unknown,
    Greater = Unknown,
    Hash = Unknown,
    LeftParen = Unknown,
    Less = Unknown,
    Percent = Unknown,
    Plus = Unknown,
    Question = Unknown,
    DoubleQuote = Unknown,
    RightParen = Unknown,
    Underscore = Unknown,
};  // enum class Keycode

enum Modifier : unsigned {
    LCTRL = 1 << 0,
    RCTRL = 1 << 1,
    LSHIFT = 1 << 2,
    RSHIFT = 1 << 3,
    LALT = 1 << 4,
    RALT = 1 << 5,
    NUM_LOCK = 1 << 6,
    SCROLL_LOCK = 1 << 7,
    CAPS_LOCK = 1 << 8,
    SHIFT = 1 << 9,
    CTRL = 1 << 10,
    ALT = 1 << 11,
};  // enum Modifier

struct EventArgs {};  // struct EventArgs

struct CloseEventArgs : public EventArgs {};  // struct CloseEventArgs

struct ResizeEventArgs : public EventArgs {
    int width;
    int height;
};  // struct ResizeEventArgs

struct MouseMotionEventArgs : public EventArgs {
    int x;
    int y;
};  // struct MouseMotionEventArgs

struct MouseWheelEventArgs : public EventArgs {
    int x;
    int y;
    int offest_x;
    int offest_y;
};  // struct MouseWheelEventArgs

struct MouseClickEventArgs : public EventArgs {
    int x;
    int y;
    MouseButton button;
    bool pressed;
    bool released;
};  // struct MouseClickEventArgs

struct KeyboardEventArgs : public EventArgs {
    Keycode code;
    unsigned modifiers;
    bool pressed;
    bool released;
};  // struct KeyboardEventArgs

typedef std::function<void(void *, EventArgs *)> CallbackType;

}  // namespace render

template <>
struct std::hash<render::EventType> {
    auto operator()(const render::EventType &value) const noexcept -> size_t {
        return static_cast<size_t>(value);
    }
};  // struct std::hash<render::EventType>

namespace render {

class Window {
 public:
    Window() = delete;
    Window(const int width,
           const int height,
           const UTF16String title,
           const Image &icon,
           const bool fullscreen = false); /*implement this*/
    ~Window();                             /*implement this*/

    Window(const Window &) = delete;
    auto operator=(const Window &) -> Window & = delete;

    Window(Window &&) = delete;
    auto operator=(Window && ) -> Window & = delete;

    void AddHandler(const EventType &type,
                    const CallbackType &callback); /*implement this*/
    void RemoveHandlers(const EventType &type);    /*implement this*/

    void Close();                                   /*implement this*/
    void Resize(const int width, const int height); /*implement this*/

    auto IsValid() const -> bool; /*implement this*/
};                                // class Window

///////////////
// Rendering //
///////////////

class Texture {
 public:
    Texture() = delete;
    Texture(const Image &image); /*implement this*/
    ~Texture();                  /*implement this*/

    Texture(const Texture &) = delete;
    auto operator=(const Texture &) -> Texture & = delete;

    Texture(Texture &&) = delete;
    auto operator=(Texture && ) -> Texture & = delete;

    auto IsValid() const -> bool; /*implement this*/
};                                // class Texture

struct Vertex {
    float x, y, z;
    float r, g, b, a;
    float u, v;
    float nx, ny, nz;
};  // struct Vertex

class VertexBuffer {
 public:
    VertexBuffer() = delete;
    ~VertexBuffer(); /*implement this*/

    VertexBuffer(const VertexBuffer &) = delete;
    auto operator=(const VertexBuffer &) -> VertexBuffer & = delete;

    VertexBuffer(VertexBuffer &&) = delete;
    auto operator=(VertexBuffer && ) -> VertexBuffer & = delete;

    auto IsValid() const -> bool; /*implement this*/
};                                // class VertexBuffer

class IndexBuffer {
 public:
    IndexBuffer() = delete;
    ~IndexBuffer(); /*implement this*/

    IndexBuffer(const IndexBuffer &) = delete;
    auto operator=(const IndexBuffer &) -> IndexBuffer & = delete;

    IndexBuffer(IndexBuffer &&) = delete;
    auto operator=(IndexBuffer && ) -> IndexBuffer & = delete;

    auto IsValid() const -> bool; /*implement this*/
};                                // class IndexBuffer

enum class ShaderType {
    VertexShader,
    PixelShader,
};  // enum class ShaderType

class Shader {
 public:
    Shader() = delete;
    Shader(const UTF16String filepath,
           const ShaderType &type); /*implement this*/
    ~Shader();                      /*implement this*/

    Shader(const Shader &) = delete;
    auto operator=(const Shader &) -> Shader & = delete;

    Shader(Shader &&) = delete;
    auto operator=(Shader && ) -> Shader & = delete;

    auto IsValid() const -> bool; /*implement this*/
};                                // class Shader

class ShaderProgram {
 public:
    ShaderProgram() = delete;
    ShaderProgram(Shader *vertex, Shader *pixel); /*implement this*/
    ~ShaderProgram();                             /*implement this*/

    ShaderProgram(const ShaderProgram &) = delete;
    auto operator=(const ShaderProgram &) -> ShaderProgram & = delete;

    ShaderProgram(ShaderProgram &&) = delete;
    auto operator=(ShaderProgram && ) -> ShaderProgram & = delete;

    auto IsValid() const -> bool; /*implement this*/
};                                // class ShaderProgram

class Renderer {
 public:
    Renderer() = delete;
    Renderer(Window *window, ShaderProgram *program); /*implement this*/
    ~Renderer();                                      /*implement this*/

    Renderer(const Renderer &) = delete;
    auto operator=(const Renderer &) -> Renderer & = delete;

    Renderer(Renderer &&) = delete;
    auto operator=(Renderer && ) -> Renderer & = delete;

    void SetProjectionMatrix(const glm::mat4 &matrix); /*implement this*/
    void SetModelMatrix(const glm::mat4 &matrix);      /*implement this*/
    void SetViewMatrix(const glm::mat4 &matrix);       /*implement this*/

    void BindTexture(const Texture &texture); /*implement this*/
    void UnbindAllTexture();                  /*implement this*/

    void ResetShaderProgram(ShaderProgram *program); /*implement this*/

    void CreateVertexBuffer(VertexBuffer *target,
                            const int size,
                            Vertex *data); /*implement this*/
    void CreateIndexBuffer(IndexBuffer *target,
                           const int size,
                           unsigned *data); /*implement this*/

    template <typename TContainer>
    void CreateVertexBuffer(VertexBuffer *target, const TContainer &data) {
        CreateVertexBuffer(target, data.size(), data.data());
    }

    template <typename TContainer>
    void CreateIndexBuffer(IndexBuffer *target, const TContainer &data) {
        CreateIndexBuffer(target, data.size(), data.data());
    }

    void Clear(const float red = 0.0f,
               const float green = 0.0f,
               const float blue = 0.0f); /*implement this*/
    void Begin();                        /*implement this*/
    void End();                          /*implement this*/
    void Present();                      /*implement this*/

    template <typename TBuffer>
    void DrawBuffer(const TBuffer &buffer); /*implement this*/
};                                          // class Renderer

}  // namespace render

#endif  // RENDER_UNIVERSAL_HPP_
```
