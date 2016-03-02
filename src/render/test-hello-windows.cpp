//
// Copyright 2016 HNSDFZ_OIER
//

#include <cassert>
#include <cstdlib>
#include <fstream>

#include "opengl330/opengl330.hpp"

using namespace std;
using namespace render;
using namespace glm;

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
    cout << "Mouse Event" << endl;
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
    atexit(Terminate);
    Image icon(u"icon.jpg");

    assert(icon.IsValid());

    cout << "Creating windows..." << endl;
    Window wnd(800, 600, u"Hello, window!", icon, RESIZABLE);
    wnd.AddHandler(EventType::Close, on_close);
    wnd.AddHandler(EventType::Close, on_close2);
    wnd.AddHandler(EventType::Resize, on_resize);
    wnd.AddHandler(EventType::MouseClick, on_mouse_click);
    wnd.AddHandler(EventType::MouseMotion, on_mouse_motion);
    wnd.AddHandler(EventType::Keyboard, on_keyboard);

    cout << "Creating shaders..." << endl;
    Shader vertex_shader(u"./opengl330/vertex.glsl", ShaderType::VertexShader);
    Shader pixel_shader(u"./opengl330/pixel.glsl", ShaderType::PixelShader);

    cout << "Creating shader program..." << endl;
    ShaderProgram program(&vertex_shader, &pixel_shader);

    cout << "Creating renderers..." << endl;
    Renderer ren(&wnd, &program);

    assert(vertex_shader.IsValid());
    assert(pixel_shader.IsValid());
    assert(program.IsValid());

    Vertex data[] = { { 0.1f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 0.0f },
                      { 0.9f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                        0.0f, 0.0f, 0.0f },
                      { 0.9f, 0.9f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f },
                      { 0.1f, 0.9f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f } };
    unsigned indices[] = { 0, 1, 2, 0, 2, 3 };

    VertexBuffer buffer;
    ren.SetVertexBuffer(&buffer, sizeof(data) / sizeof(Vertex), data,
                        PrimitiveType::Point);
    IndexBuffer indexes;
    ren.SetIndexBuffer(&indexes, &buffer, sizeof(indexes) / sizeof(unsigned),
                       indices, PrimitiveType::Triangle);
    assert(buffer.IsValid());
    assert(indexes.IsValid());

    Texture texture(icon);
    assert(texture.IsValid());

    ren.SetProjectionMatrix(ortho(0.0f, 1.0f, 0.0f, 1.0f));
    ren.SetViewMatrix(mat4());
    ren.SetModelMatrix(mat4());

    while (wnd.IsValid()) {
        DoWindowEvents();

        ren.Clear(0.0f, 0.0f, 0.0f);
        ren.Begin();

        ren.BindCurrentTexture(texture);
        ren.DrawBuffer(indexes);

        ren.End();
        ren.Present();
    }  // while

    cout << "Program exited." << endl;

    return 0;
}  // function main
