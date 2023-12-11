#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float hp;

// Output
out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;

void main()
{
    // Send output to fragment shader
    frag_normal = v_normal;
    frag_color = v_color;
    tex_coord = v_texture_coord;
    
    vec3 world_pos =  vec3 (Model * vec4(v_position, 1));
    gl_Position = Projection * View * vec4(world_pos, 1.0f);
}
