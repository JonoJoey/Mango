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
    float shine_damper;
    sampler2D diffuse_map;
    sampler2D specular_map;
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
    vec3 specular_color =vec3(texture(u_material.specular_map, _tex_coord));

    // ambient
    vec3 ambient = u_light.ambient * diffuse_color;

    // diffuse
    float diff = max(dot(normal, to_light_dir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * diffuse_color;

    // specular
    vec3 view_dir = normalize(u_camera_position - _world_position);
    vec3 halfway_dir = normalize(to_light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), u_material.shine_damper);
    vec3 specular = u_light.specular * spec * specular_color;

    out_color = vec4(ambient + diffuse + specular, 1.0);
}
