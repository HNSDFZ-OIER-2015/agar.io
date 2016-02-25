# Fake Agar.io Backend Interfaces Specific
> Version: V0.0.4
> License: MIT

## 目标
封装**底层**图形接口。
统一Direct3D和OpenGL的接口。

##使用流程：
```
载入（Initialize） -> 创建窗口 -> 载入着色器 -> 制作着色程序 -> 创建渲染器 -> 绑定着色程序 -> 设置渲染器 -> 「Application Loop」 -> [销毁资源（RAII）] -> 退出（Terminate）
```

## 命名空间
```cpp
namespace render {

}  // namespace render
```

## 特殊识别
```
#define BACKEND_UNKNOWN

// DirectX后端
#define BACKEND_DIRECT3D9
#define BACKEND_DIRECT3D10
#define BACKEND_DIRECT3D11
#define BACKEND_DIRECT3D12
#define BACKEND_DIRECT2D

// OpenGL后端
#define BACKEND_OPENGL
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

## 加载
```cpp
// 载入必要资源
void Initialize();

// 释放资源
void Terminate();
```

## 编码
统一使用UTF-16。
```cpp
typedef uint16_t CharType;
```

## 第三方库
数学库： [glm](https://github.com/g-truc/glm)

```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
```

## 资源
### 图像
```cpp
enum class ImageFormat {
    RGB, RGBA
};

class Image {
 public:
    // 从文件加载
    Image(const CharType *filepath);
    
    // 销毁
    ~Image();
    
    // 保存到文件
    void Save(const CharType *filepath);
    
    // 宽度 in pixels
    auto GetWidth() const -> int;
    
    // 高度 in pixels
    auto GetHeight() const -> int;
    
    // 获取图片内部格式
    auto GetFormat() const -> ImageFormat;
    
    // 是否有效
    auto IsValid() const -> bool;
};
```

### 字体
```cpp
class Font {
 public:
    // 从文件加载
    Font(const CharType *filepath);
    
    // 销毁
    ~Font();
    
    // 是否有效
    auto IsValid() const -> bool;
};
```

## 平台相关
### 事件处理
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
    
    // 滚轮转动量（y通常为0）
    int offest_x;
    int offest_y;
};

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

enum Modifier : unsigned {
    LCTRL, RCTRL, LSHIFT, RSHIFT, LALT, RALT, NUM_LOCK, SCROLL_LOCK, CAPS_LOCK, SUPER
    // SUPER: 在Windows上是"Windows"键
};

struct Keyboard {
    // 按键
    Keycode code;
    
    // 特殊按键（CTRL、SHIFT和ALT这些），每个按键占一个二进制位
    // 通过按位与（&）来确定某个键是否被按下
    // 例如：
    // modifier & (LCTRL | LSHIFT) == true 表明左Control和左Shift被同时按下
    unsigned modifier;
    
    // 如果按下并未释放，为true
    bool pressed;
    
    // 如果是按下后释放了，为true
    bool released;
};
```

回调函数：
```cpp
void on_event(void *sender, EventArgs *args);
```

示例
```
struct KeyboardEventArgs : public EventArgs {
    Keycode code;
};

void on_key_press(void *sender, EventArgs *args) {
    Window *window = reinterpret_cast<Window *>(sender);
    KeyboardEventArgs *event = reinterpret_cast<KeyboardEventArgs *>(args);
    
    if (event->code == Keycode.Escape) {
        window->Close();
    }
}
```

### 窗口
```cpp
class Window {
 public:
    typedef EventEnum EventType;
    typedef std::function<void(void *, EventArgs *)> CallbackType;
    
    Window(
        const int width,             // 窗口宽度
        const int height,            // 窗口高度
        const CharType *title,       // 标题
        const Image &image,          // 图标
        const bool fullscreen=false  // 是否全屏
    );
    ~Window();
    
    // 处理所有事件
    void DoEvents();
    
    // 绑定事件
    void AddHandler(const EventType event, const CallbackType callback);
    
    // 删除某一事件的**所有**绑定
    void RemoveHandlers(const EventType event);
    
    // 关闭窗口
    // 应确保会引发CloseEvent
    void Close();
    
    // 重置窗口大小
    // 应确保会引发ResizeEvent
    void Resize(const int width, const int height);
    
    auto IsValid() const -> bool;
};
```

## 渲染
### 资源
#### 材质
```cpp
class Texture {
 public:
    // 从图像载入
    Texture(const Image &image);
    ~Texture();
    
    auto IsValid() const -> bool;
};
```

#### 缓冲
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
```cpp
class Shader {
 public:
    // 从文件载入指定类型的着色器
    Shader(const CharType *filepath, const ShaderType type);
    ~Shader();
    
    auto IsValid() const -> bool;
};
```

着色程序：
着色程序是顶点着色器和像素着色器的结合体
```cpp
class ShaderProgram {
 public:
    Program(const Shader &vertex, const Shader &pixel);
    ~Program();
    
    auto IsValid() const -> bool;
};
```

### 渲染器
渲染器：
```cpp
class Renderer {
 public:
    // 创建一个绑定到指定窗口的渲染器
    Renderer(Window *window, ShaderProgram *program = nullptr);
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
    void CreateVertexBuffer(VertexBuffer &target, const TContainer<Vertex> &data) {
        CreateVertexBuffer(target, data.size(), data.data());
    }

    template <typename TContainer>
    void CreateIndexBuffer(IndexBuffer &target, const TContainer<unsigned> &data) {
        CreateIndexBuffer(target, data.size(), data.data());
    }
    
    // 清空
    void Clear(
        float red = 0.0f,
        float green = 0.0f,
        float blue = 0.0f,
        float alpha = 1.0f
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
    
    // In .cpp file
    // 顶点缓冲的特化版本
    template <>
    void DrawBuffer(const VertexBuffer &buffer);
    
    // 索引缓冲的特化版本
    template <>
    void DrawBuffer(const IndexBuffer &buffer);
};
```
