#version 330

// Input
in vec2 texcoord;
in vec3 normal;

// Uniform properties
uniform sampler2D texture;

// Output
layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture2D(texture, texcoord);

    if (out_color.a < 0.9) {
        discard;
    }
}
