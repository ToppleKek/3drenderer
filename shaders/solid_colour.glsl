#version 460 core
out vec4 frag_colour;
uniform vec4 colour;

void main() {
    frag_colour = colour;
}
