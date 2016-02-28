//
// Copyright 2016 HNSDFZ-OIER
//

#include "opengl330.hpp"

#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>

using namespace std;
using namespace glm;
using namespace render;

namespace {
// UTF-8 file path is not supported... orz
auto force_to_ascii(const UTF16String str) -> string {
    string result;

    for (size_t i = 0; str[i] != '\0'; i++) {
        CharType c = str[i];
        result.push_back(static_cast<char>(c));
    }  // for

    return result;
}

auto read_file(const UTF16String filepath) -> string {
    string path = force_to_ascii(filepath);
    ifstream file(path);
    ostringstream buffer;

    if (not file.is_open()) {
        throw runtime_error(string("Can't open file to read: ") + path);
    }

    while (file.good()) {
        string line;
        getline(file, line);
        buffer << line << "\n";
    }  // while

    return buffer.str();
}

auto get_shader_info(GLuint shader) -> string {
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    unique_ptr<GLchar> info(new GLchar[length]);
    glGetShaderInfoLog(shader, length, nullptr, info.get());

    return string(info.get());
}

auto get_program_info(GLuint program) -> string {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    unique_ptr<GLchar> info(new GLchar[length]);
    glGetProgramInfoLog(program, length, nullptr, info.get());

    return string(info.get());
}

// Key modifier
unsigned get_modifiers() {
    const uint8_t *state = SDL_GetKeyboardState(nullptr);
    unsigned modifiers = 0;

    if (state[SDL_SCANCODE_LCTRL])
        modifiers |= LCTRL;
    if (state[SDL_SCANCODE_RCTRL])
        modifiers |= RCTRL;
    if (state[SDL_SCANCODE_LSHIFT])
        modifiers |= LSHIFT;
    if (state[SDL_SCANCODE_RSHIFT])
        modifiers |= RSHIFT;
    if (state[SDL_SCANCODE_LALT])
        modifiers |= LALT;
    if (state[SDL_SCANCODE_RALT])
        modifiers |= RALT;
    if (state[SDL_SCANCODE_NUMLOCKCLEAR])
        modifiers |= NUM_LOCK;
    if (state[SDL_SCANCODE_SCROLLLOCK])
        modifiers |= SCROLL_LOCK;
    if (state[SDL_SCANCODE_CAPSLOCK])
        modifiers |= CAPS_LOCK;
    if (modifiers & (LCTRL | RCTRL))
        modifiers |= CTRL;
    if (modifiers & (LSHIFT | RSHIFT))
        modifiers |= SHIFT;
    if (modifiers & (LALT | RALT))
        modifiers |= ALT;

    return modifiers;
}

// Event handling
static unordered_map<uint32_t, Window *> window_map;
void do_events() {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_WINDOWEVENT: {
                Window *window = window_map[e.window.windowID];

                switch (e.window.event) {
                    case SDL_WINDOWEVENT_CLOSE: {
                        CloseEventArgs args;
                        window->exRaise(EventType::Close, &args);
                        window->Close();

                        break;
                    }  // case SDL_WINDOWEVENT_CLOSE

                    case SDL_WINDOWEVENT_RESIZED: {
                        ResizeEventArgs args;
                        args.width = e.window.data1;
                        args.height = e.window.data2;
                        window->exRaise(EventType::Resize, &args);

                        break;
                    }  // case SDL_WINDOWEVENT_RESIZED
                }      // switch to e.window.event

                break;
            }  // case SDL_WINDOWEVENT

            case SDL_MOUSEBUTTONDOWN: {
                Window *window = window_map[e.button.windowID];
                MouseClickEventArgs args;
                args.x = e.button.x;
                args.y = e.button.y;
                args.button = static_cast<MouseButton>(e.button.button);
                args.pressed = true;
                args.released = false;
                window->exRaise(EventType::MouseClick, &args);

                break;
            }  // case SDL_MOUSEBUTTONDOWN:

            case SDL_MOUSEBUTTONUP: {
                Window *window = window_map[e.button.windowID];
                MouseClickEventArgs args;
                args.x = e.button.x;
                args.y = e.button.y;
                args.button = static_cast<MouseButton>(e.button.button);
                args.pressed = false;
                args.released = true;
                window->exRaise(EventType::MouseClick, &args);

                break;
            }  // case SDL_MOUSEBUTTONUP

            case SDL_MOUSEMOTION: {
                Window *window = window_map[e.motion.windowID];
                MouseMotionEventArgs args;
                args.x = e.motion.x;
                args.y = e.motion.y;
                window->exRaise(EventType::MouseMotion, &args);

                break;
            }  // case SDL_MOUSEMOTION

            case SDL_MOUSEWHEEL: {
                Window *window = window_map[e.wheel.windowID];
                MouseWheelEventArgs args;
                args.offest_x = e.wheel.x;
                args.offest_y = e.wheel.y;
                SDL_GetMouseState(&args.x, &args.y);
                window->exRaise(EventType::MouseMotion, &args);

                break;
            }  // case SDL_MOUSEWHEEL

            case SDL_KEYDOWN: {
                Window *window = window_map[e.key.windowID];
                KeyboardEventArgs args;
                args.code = static_cast<Keycode>(e.key.keysym.sym);
                args.modifiers = get_modifiers();
                args.pressed = true;
                args.released = false;
                window->exRaise(EventType::Keyboard, &args);

                break;
            }

            case SDL_KEYUP: {
                Window *window = window_map[e.key.windowID];
                KeyboardEventArgs args;
                args.code = static_cast<Keycode>(e.key.keysym.sym);
                args.modifiers = get_modifiers();
                args.pressed = false;
                args.released = true;
                window->exRaise(EventType::Keyboard, &args);

                break;
            }
        }  // switch to e.type
    }      // while
}
}  // unamed namespace

namespace render {

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
}

void Terminate() {
    SDL_Quit();
}

void DoWindowEvents() {
    do_events();
}

///////////
// Image //
///////////

Image::Image(const UTF16String filepath) {
    string path = force_to_ascii(filepath);

    m_pSurface = IMG_Load(path.data());
    if (m_pSurface == nullptr) {
        throw runtime_error(string("Can't load image: ") + path);
    }
}

Image::~Image() {
    if (IsValid()) {
        SDL_FreeSurface(m_pSurface);
        m_pSurface = nullptr;
    }
}

void Image::Save(const UTF16String filepath) {
    string path = force_to_ascii(filepath);

    if (IMG_SavePNG(m_pSurface, path.data()) < 0) {
        throw runtime_error(string("Can't save image: ") + path);
    }
}

auto Image::GetWidth() const -> int {
    return m_pSurface->w;
}

auto Image::GetHeight() const -> int {
    return m_pSurface->h;
}

auto Image::IsValid() const -> bool {
    return m_pSurface != nullptr;
}

////////////
// Window //
////////////

Window::Window(const int width,
               const int height,
               const UTF16String title,
               const Image &icon,
               const bool fullscreen) {
    string window_title = force_to_ascii(title);
    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    m_fullscreen = fullscreen;
    if (fullscreen) {
        flags = flags | SDL_WINDOW_FULLSCREEN;
    }

    m_pWindow = SDL_CreateWindow(window_title.data(),
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 width,
                                 height,
                                 flags);
    if (m_pWindow == nullptr) {
        throw runtime_error("Can't create window");
    }

    SDL_SetWindowIcon(m_pWindow, icon.m_pSurface);

    window_map[SDL_GetWindowID(m_pWindow)] = this;
}

Window::~Window() {
    if (IsValid()) {
        Close();
    }
}

void Window::AddHandler(const EventType &type, const CallbackType &callback) {
    if (m_handlers.count(type) == 0) {
        m_handlers[type] = std::vector<CallbackType>();
    }

    m_handlers[type].push_back(callback);
}

void Window::RemoveHandlers(const EventType &type) {
    if (m_handlers.count(type) != 0) {
        m_handlers[type].clear();
    }
}

void Window::Close() {
    SDL_DestroyWindow(m_pWindow);
    m_pWindow = nullptr;
    window_map[SDL_GetWindowID(m_pWindow)] = nullptr;
}

void Window::Resize(const int width, const int height) {
    SDL_SetWindowSize(m_pWindow, width, height);
}

auto Window::IsValid() const -> bool {
    return m_pWindow != nullptr;
}

void Window::exRaise(const EventType &type, EventArgs *args) {
    if (m_handlers.count(type) != 0) {
        for (auto &callback : m_handlers[type]) {
            callback(this, args);
        }  // foreach in m_handlers[type]
    }
}

auto Window::exIsFullscreen() const -> bool {
    return m_fullscreen;
}

void Window::exToggleFullscreen() {
    int status;

    if (not m_fullscreen) {
        status = SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN);
    } else {
        status = SDL_SetWindowFullscreen(m_pWindow, 0);
    }

    m_fullscreen = not m_fullscreen;

    if (status < 0) {
        throw runtime_error("Can't toggle window fullscreen");
    }
}

/////////////
// Texture //
/////////////

Texture::Texture(const Image &image) {
    glGenTextures(1, &m_texture);

    if (m_texture == 0) {
        throw runtime_error("Can't create texture object");
    }

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format;
    if (image.m_pSurface->format->format == SDL_PIXELFORMAT_RGB888) {
        format = GL_RGB;
    } else {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 image.m_pSurface->w,
                 image.m_pSurface->h,
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 image.m_pSurface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    if (IsValid()) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
}

auto Texture::IsValid() const -> bool {
    return m_texture != 0;
}

//////////////////
// VertexBuffer //
//////////////////

VertexBuffer::~VertexBuffer() {
    if (IsValid()) {
        glDeleteBuffers(1, &m_buffer);
        m_buffer = 0;
    }
}

auto VertexBuffer::IsValid() const -> bool {
    return m_buffer != 0;
}

/////////////////
// IndexBuffer //
/////////////////

IndexBuffer::~IndexBuffer() {
    if (IsValid()) {
        glDeleteBuffers(1, &m_buffer);
        m_buffer = 0;
    }
}

auto IndexBuffer::IsValid() const -> bool {
    return m_buffer != 0;
}

////////////
// Shader //
////////////

Shader::Shader(const UTF16String filepath, const ShaderType &type) {
    auto content = read_file(filepath);

    if (type == ShaderType::VertexShader) {
        m_shader = glCreateShader(GL_VERTEX_SHADER);
    } else if (type == ShaderType::PixelShader) {
        m_shader = glCreateShader(GL_FRAGMENT_SHADER);
    }

    if (m_shader == 0) {
        throw runtime_error("Can't create shader object");
    }

    const GLchar *source = content.data();
    glShaderSource(m_shader, 1, &source, nullptr);
    glCompileShader(m_shader);

    GLint status;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &status);

    if (status == 0) {
        throw runtime_error(exGetLogInfo());
    }
}

Shader::~Shader() {
    if (IsValid()) {
        glDeleteShader(m_shader);
        m_shader = 0;
    }
}

auto Shader::IsValid() const -> bool {
    return m_shader != 0;
}

auto Shader::exGetLogInfo() const -> std::string {
    return get_shader_info(m_shader);
}

auto Shader::exGetShaderType() const -> ShaderType {
    GLint type;
    glGetShaderiv(m_shader, GL_SHADER_TYPE, &type);

    return static_cast<ShaderType>(type);
}

///////////////////
// ShaderProgram //
///////////////////

ShaderProgram::ShaderProgram(Shader *vertex, Shader *pixel) {
    if (vertex->exGetShaderType() != ShaderType::VertexShader) {
        throw runtime_error("Parameter 'vertex' must be a vertex shader");
    }

    if (pixel->exGetShaderType() != ShaderType::PixelShader) {
        throw runtime_error("Parameter 'pixel' must be a pixel shader");
    }

    m_program = glCreateProgram();

    if (m_program == 0) {
        throw runtime_error("Can't create shader program object");
    }

    glAttachShader(m_program, vertex->m_shader);
    glAttachShader(m_program, pixel->m_shader);
    glLinkProgram(m_program);

    GLint status;
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);

    if (status == 0) {
        throw runtime_error(exGetLogInfo());
    }
}

ShaderProgram::~ShaderProgram() {
    if (IsValid()) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

auto ShaderProgram::IsValid() const -> bool {
    return m_program != 0;
}

auto ShaderProgram::exGetLogInfo() const -> std::string {
    return get_program_info(m_program);
}

//////////////
// Renderer //
//////////////

Renderer::Renderer(Window *window, ShaderProgram *program) {
    m_pWindow = window;
    m_pProgram = program;

    m_context = SDL_GL_CreateContext(m_pWindow->m_pWindow);

    if (m_context == nullptr) {
        throw runtime_error("Can't create OpenGL context");
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw runtime_error("Can't initialize glew");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
}

Renderer::~Renderer() {
    if (m_context != nullptr) {
        SDL_GL_DeleteContext(m_context);
        m_context = nullptr;
    }
}

void Renderer::SetProjectionMatrix(const glm::mat4 &matrix) {}

void Renderer::SetModelMatrix(const glm::mat4 &matrix) {}

void Renderer::SetViewMatrix(const glm::mat4 &matrix) {}

void Renderer::BindTexture(const Texture &texture) {}

void Renderer::UnbindAllTexture() {}

void Renderer::ResetShaderProgram(ShaderProgram *program) {}

void Renderer::CreateVertexBuffer(VertexBuffer *target,
                                  const int size,
                                  Vertex *data) {}

void Renderer::CreateIndexBuffer(IndexBuffer *target,
                                 VertexBuffer *vertex,
                                 const int size,
                                 unsigned *data) {}

void Renderer::Clear(const float red, const float green, const float blue) {
    glClearColor(red, green, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Begin() {
    SDL_GL_MakeCurrent(m_pWindow->m_pWindow, m_context);
}

void Renderer::End() {}

void Renderer::Present() {
    glFlush();
    SDL_GL_SwapWindow(m_pWindow->m_pWindow);
}

template <>
void Renderer::DrawBuffer(const VertexBuffer &buffer) {}

template <>
void Renderer::DrawBuffer(const IndexBuffer &buffer) {}

}  // namespace render
