#version 460 core
out vec4 frag_colour;

in vec3 view_frag_pos;
in vec3 colour;
in vec3 normal;

uniform sampler2D entity_texture;
uniform vec3 ambient_light;
uniform vec3 test_light_pos;
uniform vec3 test_light_colour;
uniform float specular_exponent;

uniform mat4 view;

void main() {
    vec3 light_direction = normalize((view * vec4(test_light_pos, 1.0)).xyz - view_frag_pos);
    vec3 reflection      = reflect(-light_direction, normal);
    vec3 diffuse_light   = max(dot(normalize(normal), light_direction), 0.0) * test_light_colour;
    float spec           = pow(max(dot(normalize(-view_frag_pos), reflection), 0.0), specular_exponent);
    vec3 specular_light  = 0.5 * spec * test_light_colour;
    vec3 light           = ambient_light + diffuse_light + specular_light;

    // frag_colour = vec4(1.0, 0.0, 0.0, 1.0);
    // frag_colour = vec4(colour * light, 1.0);
    frag_colour = vec4(colour, 1.0);
    // frag_colour = vec4(tex_coord.x, tex_coord.y, 0.0, 1.0);
}
