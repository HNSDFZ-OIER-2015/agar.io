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

#define BACKEND_UNKNOWN

namespace render {

typedef uint16_t CharType;

void Initialize(); /*implement this*/
void Terminate();  /*implement this*/

///////////////
// Resources //
///////////////

enum class ImageFormat {
    RGB,
    RGBA,
};  // enum class ImageFormat

class Image {
 public:
    Image() = delete;
    Image(const CharType *filepath); /*implement this*/
    ~Image();                        /*implement this*/

    Image(const Image &) = delete;
    auto operator=(const Image &) -> Image & = delete;

    Image(Image &&) = delete;
    auto operator=(Image && ) -> Image & = delete;

    void Save(const CharType *filepath);   /*implement this*/
    auto GetWidth() const -> int;          /*implement this*/
    auto GetHeight() const -> int;         /*implement this*/
    auto GetFormat() const -> ImageFormat; /*implement this*/
    auto IsValid() const -> bool;          /*implement this*/
};                                         // class Image

//////////////
// Platform //
//////////////

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
    SUPER = 1 << 9
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

class Window {
 public:
    Window() = delete;
    Window(const int width,
           const int height,
           const CharType *title,
           const Image &icon,
           const bool fullscreen = false); /*implement this*/
    ~Window();                             /*implement this*/

    Window(const Window &) = delete;
    auto operator=(const Window &) -> Window & = delete;

    Window(Window &&) = delete;
    auto operator=(Window && ) -> Window & = delete;

    void DoEvents(); /*implement this*/
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
    Shader(const CharType *filepath, const ShaderType &type); /*implement this*/
    ~Shader();                                                /*implement this*/

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
               const float blue = 0.0f,
               const float alpha = 1.0f); /*implement this*/
    void Begin();                         /*implement this*/
    void End();                           /*implement this*/
    void Present();                       /*implement this*/

    template <typename TBuffer>
    void DrawBuffer(const TBuffer &buffer); /*implement this*/

    // In implementation
    // template <>
    // void Renderer::DrawBuffer(const VertexBuffer &buffer) {}
    // template <>
    // void Renderer::DrawBuffer(const IndexBuffer &buffer) {}
};  // class Renderer

}  // namespace render

// Hash for EventType
namespace std {
template <>
struct hash<render::EventType> {
    auto operator()(const render::EventType &value) const -> size_t {
        return static_cast<size_t>(value);
    }
};  // struct hash<render::EventType>
}  // namespace std

#endif  // RENDER_UNIVERSAL_HPP_
