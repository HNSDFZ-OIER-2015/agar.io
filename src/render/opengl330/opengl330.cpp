//
// Copyright 2016 HNSDFZ-OIER
//

#include "opengl330.hpp"

#include <string>
#include <stdexcept>

using namespace std;
using namespace glm;
using namespace render;

namespace {
// UTF-8 file path is not supported... orz
auto force_to_ascii(const CharType *str) -> string {
    string result;

    for (size_t i = 0; str[i] != '\0'; i++) {
        CharType c = str[i];
        result.push_back(static_cast<char>(c));
    }  // for

    return result;
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
                        window->Raise(EventType::Close, &args);
                        window->Close();

                        break;
                    }  // case SDL_WINDOWEVENT_CLOSE

                    case SDL_WINDOWEVENT_RESIZED: {
                        ResizeEventArgs args;
                        args.width = e.window.data1;
                        args.height = e.window.data2;
                        window->Raise(EventType::Resize, &args);

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
                window->Raise(EventType::MouseClick, &args);

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
                window->Raise(EventType::MouseClick, &args);

                break;
            }  // case SDL_MOUSEBUTTONUP

            case SDL_MOUSEMOTION: {
                Window *window = window_map[e.motion.windowID];
                MouseMotionEventArgs args;
                args.x = e.motion.x;
                args.y = e.motion.y;
                window->Raise(EventType::MouseMotion, &args);

                break;
            }  // case SDL_MOUSEMOTION

            case SDL_MOUSEWHEEL: {
                Window *window = window_map[e.wheel.windowID];
                MouseWheelEventArgs args;
                args.offest_x = e.wheel.x;
                args.offest_y = e.wheel.y;
                SDL_GetMouseState(&args.x, &args.y);
                window->Raise(EventType::MouseMotion, &args);

                break;
            }  // case SDL_MOUSEWHEEL

            case SDL_KEYDOWN: {
                Window *window = window_map[e.key.windowID];
                KeyboardEventArgs args;
                args.code = static_cast<Keycode>(e.key.keysym.sym);
                args.modifiers = get_modifiers();
                args.pressed = true;
                args.released = false;
                window->Raise(EventType::Keyboard, &args);

                break;
            }

            case SDL_KEYUP: {
                Window *window = window_map[e.key.windowID];
                KeyboardEventArgs args;
                args.code = static_cast<Keycode>(e.key.keysym.sym);
                args.modifiers = get_modifiers();
                args.pressed = false;
                args.released = true;
                window->Raise(EventType::Keyboard, &args);

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

///////////
// Image //
///////////

Image::Image(const CharType *filepath) {
    string path = force_to_ascii(filepath);

    m_pSurface = nullptr;

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

void Image::Save(const CharType *filepath) {
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
               const CharType *title,
               const Image &icon,
               const bool fullscreen) {
    string window_title = force_to_ascii(title);
    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    if (fullscreen) {
        flags = flags | SDL_WINDOW_FULLSCREEN;
    }

    m_pWindow = nullptr;
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

void Window::DoEvents() {
    do_events();
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

void Window::Raise(const EventType &type, EventArgs *args) {
    if (m_handlers.count(type) != 0) {
        for (auto &callback : m_handlers[type]) {
            callback(this, args);
        }  // foreach in m_handlers[type]
    }
}

}  // namespace render
