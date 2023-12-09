#version 330

// Input
// TODO(student): Get values from vertex shader
in vec2 frag_coord;
in vec3 frag_color;
in vec3 frag_normal;
in vec3 frag_position;
flat in float frag_hp;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // TODO(student): Write pixel out color
    if(frag_hp == 3.f) {
        out_color = vec4(frag_color, 1);
    } else {
        float darkenFactor = 1.f - frag_hp / 3.f;
        out_color = vec4(mix(frag_color, frag_color * 0.5f, darkenFactor), 1);
    }
}
