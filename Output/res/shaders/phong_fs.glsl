#version 330 core

layout(location = 0) out vec4 out_color;

in vec2 _tex_coord;
in vec3 _normal;
in vec3 _world_position;

struct Light
{
    vec3 position;
    vec3 color;
};
struct Material
{
    float ambient_strength;
    float specular_strength;
    float specular_shininess;
};

uniform sampler2D u_texture;
uniform Light u_light;
uniform Material u_material;
uniform vec3 u_camera_position;

void main()
{
    vec3 color = texture(u_texture, _tex_coord).xyz;
    vec3 normal = normalize(_normal);
    vec3 to_light_dir = normalize(u_light.position - _world_position);

    // ambient
    vec3 ambient_color = u_material.ambient_strength * u_light.color * color;

    // diffuse
    float diffuse = max(dot(normal, to_light_dir), 0.0);
    vec3 diffuse_color = diffuse * u_light.color * color;

    // specular
    vec3 viewDir = normalize(u_camera_position - _world_position);
    vec3 reflectDir = reflect(-to_light_dir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), u_material.specular_shininess);
    vec3 specular_color = u_material.specular_strength * specular * u_light.color;

    out_color = vec4(ambient_color + diffuse_color + specular_color, 1.0);
}
