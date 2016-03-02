#version 130

in vec3 position_data;
in vec4 color_data;
in vec2 texcoord_data;
in vec3 normal_data;

out vec4 color;
out vec2 texcoord;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(position_data.xy, 0.0, 1.0);
    color = color_data;
    texcoord = texcoord_data;
    normal = normal_data;
}
