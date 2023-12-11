#version 330

// Input
in vec3 frag_normal;
in vec2 tex_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform float hp; 

// Output
layout(location = 0) out vec4 out_color;

vec4 mix(vec4 a, vec4 b, float mix_value) {
    return b * (1.0 - mix_value) + a * mix_value;
}

void main() {
    out_color = texture(texture_1, tex_coord);
    vec4 dark_color = vec4(0.0, 0.0, 0.0, 1.0);
    
	// Apply damage shader
    out_color = mix(out_color, dark_color, hp);

    if (out_color.a < 0.9) {
        discard;
    }
}