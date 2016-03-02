#version 130

in vec4 color;
in vec2 texcoord;
in vec3 normal;

uniform int has_texture;
uniform sampler2D target;

void main() {
    if (has_texture == 1) {
        gl_FragColor = texture(target, vec2(texcoord.x, 1.0 - texcoord.y)) * color;
    } else {
        gl_FragColor = color;
    }
}
