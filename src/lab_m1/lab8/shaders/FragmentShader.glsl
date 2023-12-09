#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_positions[2];
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform float cut_off_angle;
uniform bool spotlight;
uniform vec3 object_color;
// Output
layout(location = 0) out vec4 out_color;

vec3 point_light_contribution(vec3 light_position) {
    vec3 N = normalize(world_normal);
    vec3 V = normalize(eye_position - world_position); 
    vec3 L = normalize(light_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = reflect (-L, N);

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0;
    float diffuse_light = material_kd * max(0, dot(N, L)) * 1.5f;
    float specular_light = 0;
    float emissive_light = 0;
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0) {
        specular_light = material_ks * pow(max(0, dot(N, H)), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float auxiliar = ambient_light + diffuse_light + specular_light; // kindish ambient_light + diffuse_light + specular_light
    if (spotlight) {
        float cut_off = radians(cut_off_angle);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);
 
        if (spot_light > cos(cut_off)) {
	        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);

            auxiliar = ambient_light + light_att_factor * (diffuse_light + specular_light);
        } else {
            auxiliar = ambient_light;
        }
    } else {
        float dist = distance(world_position, light_position);
        float att_factor = 1.0 / (pow(dist, 2) + 1);
        auxiliar = ambient_light + att_factor * (diffuse_light + specular_light);
    }

    // TODO(student): Write pixel out color
    return vec3(object_color * (auxiliar + emissive_light));

}

void main() {
    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    for (int i = 0; i < 2; i++) {
        out_color += vec4(point_light_contribution(light_positions[i]), 1);
    }
}
