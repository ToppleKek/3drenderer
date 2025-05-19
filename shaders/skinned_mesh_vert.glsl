#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in ivec4 bones;
layout (location = 4) in vec4 weights;

out vec2 tex_coord;
out vec3 normal;
out vec3 view_frag_pos;


uniform mat4 model;
// uniform mat4 normal_model;
uniform mat4 view;
uniform mat4 proj;

layout (std140, binding = 0, row_major) uniform Bones {
    mat4 final_xforms[400];
    mat4 inverse_bind_matrices[400];
};

void main() {
    tex_coord     = tex;
    // normal        = mat3(transpose(inverse(model))) * norm;

    vec4 transformed_pos    = vec4(0.0);
    vec3 transformed_normal = vec3(0.0);
    for (int i = 0; i < 4; ++i) {
        if (weights[i] == 0.0) break;

        vec4 p = final_xforms[bones[i]] * inverse_bind_matrices[bones[i]] * vec4(pos, 1.0);
        vec3 n = transpose(inverse(mat3(final_xforms[bones[i]]))) * mat3(inverse_bind_matrices[bones[i]]) * norm;
        transformed_pos    += p * weights[i];
        transformed_normal += n * weights[i];
    }

    // normal        = mat3(view) * mat3(model) * norm;
    normal        = mat3(view) * mat3(model) * transformed_normal;
    view_frag_pos = (view * model * transformed_pos).xyz;
    // gl_Position = vec4(pos, 1.0);
    gl_Position = proj * view * model * transformed_pos;
    // gl_Position = view * vec4(pos, 1.0);
}
