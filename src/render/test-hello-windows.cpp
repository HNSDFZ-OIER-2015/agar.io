//
// Copyright 2016 HNSDFZ_OIER
//

#include <cassert>
#include <fstream>

#include "opengl330/opengl330.hpp"

using namespace std;
using namespace render;

static ofstream cout("output.log");

namespace {
void on_close(void *sender, EventArgs * /*args*/) {
    // Window *wnd = reinterpret_cast<Window *>(sender);

    // wnd->Close();
    cout << "Close event at " << sender << endl;
}

void on_close2(void * /*sender*/, EventArgs * /*args*/) {
    cout << "Multiple callbacks" << endl;
}

void on_resize(void *sender, EventArgs *args) {
    ResizeEventArgs *event = reinterpret_cast<ResizeEventArgs *>(args);

    cout << "Window at " << sender << " resized to " << event->width << "x"
         << event->height << endl;
}

void on_mouse_click(void *sender, EventArgs *args) {
    MouseClickEventArgs *event = reinterpret_cast<MouseClickEventArgs *>(args);

    cout << "Window at " << sender << " clicked: x = " << event->x
         << ", y = " << event->y << boolalpha
         << ", pressed = " << event->pressed
         << ", released = " << event->released << endl;

    if (event->button == MouseButton::Left) {
        cout << "Left" << endl;
    } else if (event->button == MouseButton::Middle) {
        cout << "Middle" << endl;
    } else if (event->button == MouseButton::Right) {
        cout << "Right" << endl;
    }
}

void on_mouse_motion(void * /*sender*/, EventArgs * /*args*/) {
    // MouseMotionEventArgs *event =
    //     reinterpret_cast<MouseMotionEventArgs *>(args);

    // cout << "Window at " << sender << " moved: x = " << event->x
    //      << ", y = " << event->y << endl;
}

void on_keyboard(void *sender, EventArgs *args) {
    Window *window = reinterpret_cast<Window *>(sender);
    KeyboardEventArgs *event = reinterpret_cast<KeyboardEventArgs *>(args);

    cout << "Window at " << sender
         << " key event: code = " << static_cast<int>(event->code)
         << ", modifiers = " << event->modifiers << boolalpha
         << ", pressed = " << event->pressed
         << ", released = " << event->released << endl;

    if (event->released)
        return;

    if (event->code == Keycode::Escape) {
        window->Close();
    }

#ifdef BACKEND_OPENGL330
    if (event->code == Keycode::F11) {
        window->exToggleFullscreen();
    }
#endif
}
}

int main() {
    cout << "Hello, window!" << endl;

    Initialize();
    Image icon(u"icon.jpg");

    assert(icon.IsValid());

    cout << "Creating windows..." << endl;
    Window wnd1(800, 600, u"Hello, window #1!", icon);
    wnd1.AddHandler(EventType::Close, on_close);
    wnd1.AddHandler(EventType::Close, on_close2);
    wnd1.AddHandler(EventType::Resize, on_resize);
    wnd1.AddHandler(EventType::MouseClick, on_mouse_click);
    wnd1.AddHandler(EventType::MouseMotion, on_mouse_motion);
    wnd1.AddHandler(EventType::Keyboard, on_keyboard);

    Window wnd2(800, 600, u"Hello, window #2!", icon);
    wnd2.AddHandler(EventType::Close, on_close);
    wnd2.AddHandler(EventType::Close, on_close2);
    wnd2.AddHandler(EventType::Resize, on_resize);
    wnd2.AddHandler(EventType::MouseClick, on_mouse_click);
    wnd2.AddHandler(EventType::MouseMotion, on_mouse_motion);
    wnd2.AddHandler(EventType::Keyboard, on_keyboard);

    cout << "Creating shaders..." << endl;
    Shader vertex_shader(u"./opengl330/vertex.glsl", ShaderType::VertexShader);
    Shader pixel_shader(u"./opengl330/pixel.glsl", ShaderType::PixelShader);

    cout << "Creating shader program..." << endl;
    ShaderProgram program(&vertex_shader, &pixel_shader);

    cout << "Creating renderers..." << endl;
    Renderer ren1(&wnd1, &program);
    Renderer ren2(&wnd2, &program);

    assert(vertex_shader.IsValid());
    assert(pixel_shader.IsValid());
    assert(program.IsValid());

    while (wnd1.IsValid() or wnd2.IsValid()) {
        DoWindowEvents();

        ren1.Begin();
        ren1.Clear();
        ren1.End();
        ren1.Present();

        ren2.Begin();
        ren2.Clear(1.0f, 1.0f, 1.0f);
        ren2.End();
        ren2.Present();
    }  // while

    Terminate();

    cout << "Program exited." << endl;

    return 0;
}  // function main
