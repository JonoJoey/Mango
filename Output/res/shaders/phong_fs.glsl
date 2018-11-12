#version 330 core

layout(location = 0) out vec4 out_color;

in vec2 _tex_coord;
in vec3 _normal;
in vec3 _world_position;

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct Material
{
    float specular_strength;
    float specular_shininess;
    sampler2D diffuse_map;
};

uniform Light u_light;
uniform Material u_material;
uniform vec3 u_camera_position;

void main()
{
    vec3 normal = normalize(_normal);
    vec3 to_light_dir = normalize(u_light.position - _world_position);

    // textures
    vec3 diffuse_color = vec3(texture(u_material.diffuse_map, _tex_coord));

    // ambient
    vec3 ambient = u_light.ambient * diffuse_color;

    // diffuse
    float diff = max(dot(normal, to_light_dir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * diffuse_color;

    // specular
    vec3 viewDir = normalize(u_camera_position - _world_position);
    vec3 reflectDir = reflect(-to_light_dir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.specular_shininess);
    vec3 specular = u_light.specular * u_material.specular_strength * spec;

    out_color = vec4(ambient + diffuse + specular, 1.0);
}
