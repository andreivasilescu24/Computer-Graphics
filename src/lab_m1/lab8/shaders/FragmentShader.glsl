#version 330

// Input
in vec3 world_position;
in vec3 world_normal;


// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec3 light_position2;
uniform vec3 light_direction2;
uniform vec3 eye_position2;


uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int spot;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

vec3 func() {
 vec3 L = normalize( light_position - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    float d = distance( light_position, world_position );

    // TODO(student): Define ambient light component
    float ambient_light = 0.25;

    // TODO(student): Compute diffuse light component
    float diffuse_light = material_kd * max(dot(world_normal, L), 0);

    // TODO(student): Compute specular light component
    float specular_light = 0;
    if(diffuse_light > 0) {
        specular_light = material_ks * diffuse_light * pow(max(dot(world_normal, H), 0), material_shininess);
    }
    
    float cut_off = radians(30.f);
    float spot_light = dot(-L, light_direction);
    float spot_light_limit = cos(cut_off);
     
    // Quadratic attenuation
    float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
    float light_att_factor = pow(linear_att, 2);
    float fac_atenuare = 1 / pow(d, 2);
    vec3 light;
    
    if(spot != 0) {
        float spot_light_limit = cos(cut_off);
        if(spot_light > spot_light_limit) {
            light = object_color * (ambient_light + (light_att_factor + fac_atenuare) * (diffuse_light + specular_light));
        } else {
            light = ambient_light * object_color;
        }
    } else {
        light = object_color * (ambient_light + fac_atenuare * (diffuse_light + specular_light));
    }
    
    return light;
 }

void main()
{
    vec3 L = normalize( light_position - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    float d = distance( light_position, world_position );

    // TODO(student): Define ambient light component
    float ambient_light = 0.25;

    // TODO(student): Compute diffuse light component
    float diffuse_light = material_kd * max(dot(world_normal, L), 0);

    // TODO(student): Compute specular light component
    float specular_light = 0;
    if(diffuse_light > 0) {
        specular_light = material_ks * diffuse_light * pow(max(dot(world_normal, H), 0), material_shininess);
    }
    
    float cut_off = radians(30.f);
    float spot_light = dot(-L, light_direction);
    float spot_light_limit = cos(cut_off);
     
    // Quadratic attenuation
    float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
    float light_att_factor = pow(linear_att, 2);
    float fac_atenuare = 1 / pow(d, 2);
    vec3 light;
    
    if(spot != 0) {
        float spot_light_limit = cos(cut_off);
        if(spot_light > spot_light_limit) {
            light = object_color * (ambient_light + (light_att_factor + fac_atenuare) * (diffuse_light + specular_light));
        } else {
            light = ambient_light * object_color;
        }
    } else {
        light = object_color * (ambient_light + fac_atenuare * (diffuse_light + specular_light));
    }
    
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    
    
    //float fac_atenuare = 1 / pow(d, 2);
    // TODO(student): Compute light
    //float light = ambient_light + fac_atenuare * (diffuse_light + specular_light);

    // TODO(student): Send color light output to fragment shader
    //vec3 color = object_color * light;
    out_color = vec4(light, 1);
    
}
