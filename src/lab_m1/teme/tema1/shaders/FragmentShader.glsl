#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture;
uniform float time;

// Output
layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture2D(texture, texcoord); 
}
