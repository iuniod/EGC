#version 330

// Input
in vec3 frag_normal;
in vec2 tex_coord;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    out_color = texture(texture_1, tex_coord);
    if(out_color.a < 0.9)
    {
        discard;
    }
}
