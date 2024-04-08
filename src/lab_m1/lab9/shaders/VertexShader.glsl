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
uniform sampler2D texture_1;


// Output
out vec2 texcoord;
uniform bool is_heightmap;


void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
   texcoord = v_texture_coord;
    if (is_heightmap) {
        // Obține înălțimile de la pixelul curent, din dreapta și de sus
        float height = texture(texture_1, texcoord).r;
        float heightRight = texture(texture_1, texcoord + vec2(1.0/textureSize(texture_1, 0).x, 0)).r;
        float heightUp = texture(texture_1, texcoord + vec2(0, 1.0/textureSize(texture_1, 0).y)).r;

        // Calculează diferențele pe x și pe z
        float Hx = height - heightRight;
        float Hz = height - heightUp;

        // Calculează normala
        vec3 normal = normalize(vec3(Hx, 1.0, Hz));

        // Poți folosi normala pentru a influența culoarea în funcție de iluminare
        vec3 position = v_position;
        position.y = height * 6;
        position.xz += normal.xz * 0.1;
        gl_Position = Projection * View * Model * vec4(position, 1.0);

    } else {
        gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    }
}