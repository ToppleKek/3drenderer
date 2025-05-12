#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec2 tex_coord;
out vec3 normal;
out vec3 view_frag_pos;

uniform mat4 model;
// uniform mat4 normal_model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    tex_coord     = tex;
    // normal        = mat3(transpose(inverse(model))) * norm;
    normal        = mat3(view) * mat3(model) * norm;
    view_frag_pos = (view * model * vec4(pos, 1.0)).xyz;
    // gl_Position = vec4(pos, 1.0);
    gl_Position = proj * view * model * vec4(pos, 1.0);
    // gl_Position = view * vec4(pos, 1.0);
}
