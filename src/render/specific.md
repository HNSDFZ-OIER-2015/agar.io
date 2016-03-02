# Fake Agar.io Render Interfaces Specific
> Version: V0.1.53
> License: MIT

## 目标
封装**底层**图形接口。
统一Direct3D和OpenGL的接口。

##使用流程：
```
载入（Initialize） -> 创建窗口 -> 载入着色器 -> 制作着色程序 -> 创建渲染器 -> 绑定着色程序 -> 设置渲染器 -> 加载资源（纹理贴图、缓冲等） -> 「Application Loop」 -> [销毁资源（RAII）] -> 退出（Terminate）
```

## 命名空间
所有的接口统一定义在此命名空间
```cpp
namespace render {

}  // namespace render
```

## 内存管理
使用C++的RAII机制。
所有的资源使用**构造函数**创建，使用**析构函数**销毁。

## 特殊识别
使用`BACKEND_*`来表示实现的API。
```
#define BACKEND_UNKNOWN

// DirectX后端
// 所有Direct3D的API都要申明BACKEND_DIRECT3D
#define BACKEND_DIRECT3D

#define BACKEND_DIRECT3D9
#define BACKEND_DIRECT3D10
#define BACKEND_DIRECT3D11
#define BACKEND_DIRECT3D12

#define BACKEND_DIRECT2D

// OpenGL后端
// 所有OpenGL的API都要申明BACKEND_OPENGL
#define BACKEND_OPENGL

#define BACKEND_OPENGL210
#define BACKEND_OPENGL330
#define BACKEND_OPENGL450

// OpenGL ES后端
// 所有OpenGL ES的API都要申明BACKEND_OPENGLES
#define BACKEND_OPENGLES

#define BACKEND_OPENGLES1
#define BACKEND_OPENGLES2
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

**注意**：不推荐自定义`CharType`为`wchar_t`，因为在不同的平台上`wchar_t`的大小会有所不同。最好是使用硬性规定好的`char16_t`或`uint16_t`。

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
enum WindowFlags {
    DEFAULT_FLAGS,
    FULLSCREEN,  // 全屏
    RESIZABLE,   // 可调整大小
};

class Window {
 public:
    Window(
        const int width,                         // 窗口宽度
        const int height,                        // 窗口高度
        const UTF16String title,                 // 标题
        const Image &icon,                       // 图标
        const WindowFlags flags = DEFAULT_FLAGS  // 窗口参数
    );
    ~Window();
    
    // 将回调函数绑定到某一事件上
    void AddHandler(const EventType &type, const CallbackType &callback);
    
    // 删除某一事件的**所有**绑定的回调函数
    void RemoveHandlers(const EventType &type);
    
    // 关闭窗口
    // 应确保会引发CloseEvent
    void Close();
    
    // 获取目前窗口的宽度和高度
    auto GetWidth() const -> int;
    auto GetHeight() const -> int;
    
    // 重置窗口大小
    // 同时会内部分辨率
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
### 纹理
纹理是指存储在显存上的资源。
在Direct3D中称为“贴图”。
纹理的坐标从左上角开始，即左上角为$(0, 0)$，右下角为$(1, 1)$。
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
结构体内成员的顺序不应改变。
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

图元类型，表示缓冲顶点的存储格式。
```cpp
enum class PrimitiveType {
    // 未指定
    Unknown,
    // 表示存储的是点
    Point,
    // 表示存储的是每两个定点确定一条线段
    Line,
    // 表示前后两个顶点确定一条线段，类似于链条
    LineStrip
    // 表示每三个顶点确定一个三角形
    Triangle,
    // 表示每相邻的三个顶点确定一个三角形，即三角形扇
    TriangleStrip,
};
```

通过构造函数创建后的缓冲**依然处于无效状态**。
需要通过`Renderer`来将数据存储到缓冲。

顶点缓冲专门存储顶点数据，可以直接按照存储顶点的顺序来绘制。
```cpp
class VertexBuffer {
 public:
    VertexBuffer();
    ~VertexBuffer();
    
    auto IsValid() const -> bool;
};
```

索引缓冲与顶点缓冲相绑定，其中每一个值表示在顶点缓冲中的索引值。
索引缓冲可以直接按照索引的顺序来绘制。
```cpp
class IndexBuffer {
 public:
    IndexBuffer();
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
    
    // 投影矩阵、视角矩阵和模型矩阵
    void SetProjectionMatrix(const mat4 &matrix);
    void SetViewMatrix(const mat4 &matrix);
    void SetModelMatrix(const mat4 &matrix);
    
    // 设置纹理
    // 每次只能设置一个纹理
    void BindCurrentTexture(const Texture &texture);
    // 取消绑定的纹理
    void UnbindTexture();
    
    // 重设着色程序
    // 切换着色程序需要重新设置渲染器
    void ResetShaderProgram(ShaderProgram *program);
    
    // 设置缓冲中的数据
    // 注意：size表示数据中的顶点数
    void SetVertexBuffer(VertexBuffer *target,
                         const int size,
                         Vertex *data,
                         const PrimitiveType type = PrimitiveType::Triangle);
    void SetIndexBuffer(IndexBuffer *target,
                        VertexBuffer *vertex,
                        const int size,
                        unsigned *data,
                        const PrimitiveType type = PrimitiveType::Triangle);

    // 用STL容器来设置缓冲数据（已实现）
    template <typename TContainer>
    void SetVertexBuffer(VertexBuffer *target,
                         const TContainer &data,
                         const PrimitiveType type = PrimitiveType::Triangle) {
        SetVertexBuffer(target, data.size(), data.data(), type);
    }
    
    template <typename TContainer>
    void SetIndexBuffer(IndexBuffer *target,
                        VertexBuffer *vertex,
                        const TContainer &data,
                        const PrimitiveType type = PrimitiveType::Triangle) {
        SetIndexBuffer(target, vertex, data.size(), data.data(), type);
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
```

## 通用接口
通用接口请见[universal.cpp](https://raw.githubusercontent.com/HNSDFZ-OIER-2015/agar.io/render-dev/src/render/universal.hpp)，需要C++11支持。
所有需要实现者实现的部分均用`/*implement this*/`做了标记。
