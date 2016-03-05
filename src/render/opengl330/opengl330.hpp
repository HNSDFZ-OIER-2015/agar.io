//
// Copyright 2016 HNSDFZ-OIER
// NOTICE: All extensions' names start with "ex".
//

#ifndef RENDER_OPENGL330_HPP_
#define RENDER_OPENGL330_HPP_

#define BACKEND_OPENGL
#define BACKEND_OPENGL330

#ifdef __UNIX__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>
#endif  // IFDEF __UNIX__

#include <cstdint>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

// thirdparty library glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wheader-hygiene"
#endif  // __clang__

using namespace glm;

#ifdef __clang__
#pragma clang diagnostic pop
#endif  // __clang__

namespace render {
typedef char16_t CharType;
typedef const CharType *UTF16String;

void Initialize();
void Terminate();

///////////////
// Resources //
///////////////

class Image {
 public:
    Image();
    Image(const UTF16String filepath);
    ~Image();

    Image(const Image &) = delete;
    auto operator=(const Image &) -> Image & = delete;

    Image(Image &&rhs);
    auto operator=(Image &&rhs) -> Image & = delete;

    void Save(const UTF16String filepath);
    auto GetWidth() const -> int;
    auto GetHeight() const -> int;
    auto IsValid() const -> bool;

 private:
    friend class Window;
    friend class Texture;

    SDL_Surface *m_pSurface = nullptr;
};  // class Image

//////////////
// Platform //
//////////////

void DoWindowEvents();

enum class EventType {
    Close,
    Resize,
    MouseMotion,
    MouseWheel,
    MouseClick,
    Keyboard,
};  // enum class EventType

enum class MouseButton {
    Left = SDL_BUTTON_LEFT,
    Middle = SDL_BUTTON_MIDDLE,
    Right = SDL_BUTTON_RIGHT,
};  // enum class MouseButton

enum class Keycode {
    Unknown = 0,
    A = SDLK_a,
    B = SDLK_b,
    C = SDLK_c,
    D = SDLK_d,
    E = SDLK_e,
    F = SDLK_f,
    G = SDLK_g,
    H = SDLK_h,
    I = SDLK_i,
    J = SDLK_j,
    K = SDLK_k,
    L = SDLK_l,
    M = SDLK_m,
    N = SDLK_n,
    O = SDLK_o,
    P = SDLK_p,
    Q = SDLK_q,
    R = SDLK_r,
    S = SDLK_s,
    T = SDLK_t,
    U = SDLK_u,
    V = SDLK_v,
    W = SDLK_w,
    X = SDLK_x,
    Y = SDLK_y,
    Z = SDLK_z,
    Num0 = SDLK_0,
    Num1 = SDLK_1,
    Num2 = SDLK_2,
    Num3 = SDLK_3,
    Num4 = SDLK_4,
    Num5 = SDLK_5,
    Num6 = SDLK_6,
    Num7 = SDLK_7,
    Num8 = SDLK_8,
    Num9 = SDLK_9,
    Numpad0 = SDLK_KP_0,
    Numpad1 = SDLK_KP_1,
    Numpad2 = SDLK_KP_2,
    Numpad3 = SDLK_KP_3,
    Numpad4 = SDLK_KP_4,
    Numpad5 = SDLK_KP_5,
    Numpad6 = SDLK_KP_6,
    Numpad7 = SDLK_KP_7,
    Numpad8 = SDLK_KP_8,
    Numpad9 = SDLK_KP_9,
    Quote = SDLK_QUOTE,
    Backslash = SDLK_BACKSLASH,
    Backspace = SDLK_BACKSPACE,
    CapsLock = SDLK_CAPSLOCK,
    Comma = SDLK_COMMA,
    Delete = SDLK_DELETE,
    Down = SDLK_DOWN,
    End = SDLK_END,
    Equal = SDLK_EQUALS,
    Escape = SDLK_ESCAPE,
    F1 = SDLK_F1,
    F2 = SDLK_F2,
    F3 = SDLK_F3,
    F4 = SDLK_F4,
    F5 = SDLK_F5,
    F6 = SDLK_F6,
    F7 = SDLK_F7,
    F8 = SDLK_F8,
    F9 = SDLK_F9,
    F10 = SDLK_F10,
    F11 = SDLK_F11,
    F12 = SDLK_F12,
    F13 = SDLK_F13,
    F14 = SDLK_F14,
    F15 = SDLK_F15,
    F16 = SDLK_F16,
    F17 = SDLK_F17,
    F18 = SDLK_F18,
    F19 = SDLK_F19,
    F20 = SDLK_F20,
    F21 = SDLK_F21,
    F22 = SDLK_F22,
    F23 = SDLK_F23,
    F24 = SDLK_F24,
    Backquote = SDLK_BACKQUOTE,
    Home = SDLK_HOME,
    Insert = SDLK_INSERT,
    LeftAlt = SDLK_LALT,
    LeftControl = SDLK_LCTRL,
    Left = SDLK_LEFT,
    Leftbracket = SDLK_LEFTBRACKET,
    LeftShift = SDLK_LSHIFT,
    Minus = SDLK_MINUS,
    NumLock = SDLK_NUMLOCKCLEAR,
    PageDown = SDLK_PAGEDOWN,
    PageUp = SDLK_PAGEUP,
    Period = SDLK_PERIOD,
    PrintScreen = SDLK_PRINTSCREEN,
    RightAlt = SDLK_RALT,
    RightControl = SDLK_RCTRL,
    RightShift = SDLK_RSHIFT,
    Return = SDLK_RETURN,
    Right = SDLK_RIGHT,
    Rightbracket = SDLK_RIGHTBRACKET,
    ScrollLock = SDLK_SCROLLLOCK,
    Semicolon = SDLK_SEMICOLON,
    Slash = SDLK_SLASH,
    Space = SDLK_SPACE,
    Tab = SDLK_TAB,
    Up = SDLK_UP,
    Ampersand = SDLK_AMPERSAND,
    Asterisk = SDLK_ASTERISK,
    At = SDLK_AT,
    Caret = SDLK_CARET,
    Colon = SDLK_COLON,
    Dollar = SDLK_DOLLAR,
    Exclaim = SDLK_EXCLAIM,
    Greater = SDLK_GREATER,
    Hash = SDLK_HASH,
    LeftParen = SDLK_LEFTPAREN,
    Less = SDLK_LESS,
    Percent = SDLK_PERCENT,
    Plus = SDLK_PLUS,
    Question = SDLK_QUESTION,
    DoubleQuote = SDLK_QUOTEDBL,
    RightParen = SDLK_RIGHTPAREN,
    Underscore = SDLK_UNDERSCORE,
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

struct CloseEventArgs final : public EventArgs {};  // struct CloseEventArgs

struct ResizeEventArgs final : public EventArgs {
    int width;
    int height;
};  // struct ResizeEventArgs

struct MouseMotionEventArgs final : public EventArgs {
    int x;
    int y;
};  // struct MouseMotionEventArgs

struct MouseWheelEventArgs final : public EventArgs {
    int x;
    int y;
    int offest_x;
    int offest_y;
};  // struct MouseWheelEventArgs

struct MouseClickEventArgs final : public EventArgs {
    int x;
    int y;
    MouseButton button;
    bool pressed;
    bool released;
};  // struct MouseClickEventArgs

struct KeyboardEventArgs final : public EventArgs {
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

enum WindowFlags : uint32_t {
    DEFAULT_FLAGS = SDL_WINDOW_OPENGL,
    FULLSCREEN = DEFAULT_FLAGS | SDL_WINDOW_FULLSCREEN,
    RESIZABLE = DEFAULT_FLAGS | SDL_WINDOW_RESIZABLE,
};

class Window {
 public:
    Window();
    Window(const int width,
           const int height,
           const UTF16String title,
           const Image &icon,
           const WindowFlags flags = DEFAULT_FLAGS);
    ~Window();

    Window(const Window &) = delete;
    auto operator=(const Window &) -> Window & = delete;

    Window(Window &&rhs);
    auto operator=(Window && ) -> Window & = delete;

    void AddHandler(const EventType &type, const CallbackType &callback);
    void RemoveHandlers(const EventType &type);

    void Close();
    void Resize(const int width, const int height);

    auto GetWidth() const -> int;
    auto GetHeight() const -> int;

    auto IsValid() const -> bool;

    void exRaise(const EventType &type, EventArgs *args);

    auto exIsFullscreen() const -> bool;
    void exToggleFullscreen();

    static void exGetMousePosition(int &to_x, int &to_y);

 protected:
    friend class Renderer;

    SDL_Window *m_pWindow = nullptr;

 private:
    std::unordered_map<EventType, std::vector<CallbackType>> m_handlers;
    bool m_fullscreen = false;
};  // class Window

///////////////
// Rendering //
///////////////

class Texture {
 public:
    Texture();
    Texture(const Image &image);
    ~Texture();

    Texture(const Texture &) = delete;
    auto operator=(const Texture &) -> Texture & = delete;

    Texture(Texture &&rhs);
    auto operator=(Texture && ) -> Texture & = delete;

    auto IsValid() const -> bool;

 protected:
    friend class Renderer;

    GLuint m_texture = 0;
};  // class Texture

enum class PrimitiveType {
    Unknown = GL_TRIANGLES,
    Point = GL_POINTS,
    Line = GL_LINES,
    LineStrip = GL_LINE_STRIP,
    Triangle = GL_TRIANGLES,
    TriangleStrip = GL_TRIANGLE_STRIP,
};  // enum class PrimitiveType

struct Vertex {
    float x, y, z;
    float r, g, b, a;
    float u, v;
    float nx, ny, nz;

    constexpr static GLuint NumberOfAttributes = 12;
};  // struct Vertex

class VertexBuffer {
 public:
    VertexBuffer();
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer &) = delete;
    auto operator=(const VertexBuffer &) -> VertexBuffer & = delete;

    VertexBuffer(VertexBuffer &&rhs);
    auto operator=(VertexBuffer && ) -> VertexBuffer & = delete;

    auto IsValid() const -> bool;

 protected:
    friend class Renderer;

    GLuint m_vao = 0;
    GLuint m_buffer = 0;
    GLuint m_size = 0;
    GLenum m_type = GL_TRIANGLES;
};  // class VertexBuffer

class IndexBuffer {
 public:
    IndexBuffer();
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer &) = delete;
    auto operator=(const IndexBuffer &) -> IndexBuffer & = delete;

    IndexBuffer(IndexBuffer &&rhs);
    auto operator=(IndexBuffer && ) -> IndexBuffer & = delete;

    auto IsValid() const -> bool;

 protected:
    friend class Renderer;

    GLuint m_vao = 0;
    GLuint m_buffer = 0;
    GLuint m_size = 0;
    GLenum m_type = GL_TRIANGLES;
    VertexBuffer *m_pVertex = nullptr;
};  // class IndexBuffer

enum class ShaderType {
    VertexShader = GL_VERTEX_SHADER,
    PixelShader = GL_FRAGMENT_SHADER,
};  // enum class ShaderType

class Shader {
 public:
    Shader();
    Shader(const UTF16String filepath, const ShaderType &type);
    ~Shader();

    Shader(const Shader &) = delete;
    auto operator=(const Shader &) -> Shader & = delete;

    Shader(Shader &&rhs);
    auto operator=(Shader && ) -> Shader & = delete;

    auto IsValid() const -> bool;

    auto exGetLogInfo() const -> std::string;
    auto exGetShaderType() const -> ShaderType;

 private:
    friend class ShaderProgram;

    void Initialize();

    GLuint m_shader = 0;
    std::u16string m_filepath;
    ShaderType m_type;
};  // class Shader

class ShaderProgram {
 public:
    ShaderProgram();
    ShaderProgram(Shader *vertex, Shader *pixel);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram &) = delete;
    auto operator=(const ShaderProgram &) -> ShaderProgram & = delete;

    ShaderProgram(ShaderProgram &&rhs);
    auto operator=(ShaderProgram && ) -> ShaderProgram & = delete;

    auto IsValid() const -> bool;

    auto exGetLogInfo() const -> std::string;

    void Initialize();

 protected:
    friend class Renderer;

    GLuint m_program = 0;

 private:
    Shader *m_pVertex = nullptr;
    Shader *m_pPixel = nullptr;
};  // class ShaderProgram

enum class exCullFaceMode {
    NoCullFace,
    CW = GL_CW,
    CCW = GL_CCW,
};  // enum class CullFaceMode

class Renderer {
 public:
    Renderer();
    Renderer(Window *window, ShaderProgram *program);
    ~Renderer();

    Renderer(const Renderer &) = delete;
    auto operator=(const Renderer &) -> Renderer & = delete;

    Renderer(Renderer &&rhs);
    auto operator=(Renderer && ) -> Renderer & = delete;

    void SetProjectionMatrix(const glm::mat4 &matrix);
    void SetModelMatrix(const glm::mat4 &matrix);
    void SetViewMatrix(const glm::mat4 &matrix);

    void BindCurrentTexture(const Texture &texture);
    void UnbindTexture();

    void ResetShaderProgram(ShaderProgram *program);

    void SetVertexBuffer(VertexBuffer *target,
                         const int size,
                         Vertex *data,
                         const PrimitiveType type = PrimitiveType::Triangle);
    void SetIndexBuffer(IndexBuffer *target, VertexBuffer *vertex,
                        const int size, unsigned *data,
                        const PrimitiveType type = PrimitiveType::Triangle);

    template <typename TContainer>
    void SetVertexBuffer(VertexBuffer *target, const TContainer &data,
                         const PrimitiveType type = PrimitiveType::Triangle) {
        SetVertexBuffer(target, data.size(), data.data(), type);
    }

    template <typename TContainer>
    void SetIndexBuffer(IndexBuffer *target, VertexBuffer *vertex,
                        const TContainer &data,
                        const PrimitiveType type = PrimitiveType::Triangle) {
        SetIndexBuffer(target, vertex, data.size(), data.data(), type);
    }

    void Begin();
    void Clear(const float red = 0.0f,
               const float green = 0.0f,
               const float blue = 0.0f);
    void End();
    void Present();

    template <typename TBuffer>
    void DrawBuffer(const TBuffer &buffer);

    void exSetCullFace(const exCullFaceMode &mode);

 private:
    Window *m_pWindow = nullptr;
    ShaderProgram *m_pProgram = nullptr;
    SDL_GLContext m_context = nullptr;
};  // class Renderer

}  // namespace render

#endif  // RENDER_OPENGL330_HPP_
