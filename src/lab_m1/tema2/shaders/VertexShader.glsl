#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_color;
layout(location = 2) in vec2 v_coord;
layout(location = 1) in vec3 v_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Color;
uniform float hp;
uniform bool displace;

// Output
// TODO(student): Output values to fragment shader
out vec2 frag_coord;
out vec3 frag_normal;
out vec3 frag_color;
out vec3 frag_position;
flat out float frag_hp;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_position = v_position;
	frag_color = Color;
	frag_normal = v_normal;
	frag_coord = v_coord;
	frag_hp = hp;
	
    // TODO(student): Compute gl_Position
    
    if(displace == true)
        gl_Position = Projection * View * Model * vec4(v_position + (0.25f * (3.f - hp)), 1.0);
    else
        gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
