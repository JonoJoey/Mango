#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_tex_coord;
layout(location = 2) in vec3 in_normal;

out vec2 _tex_coord;
out vec3 _normal;
out vec3 _world_position;

struct Light
{
    vec3 position;
    vec3 color;
};

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform mat3 u_normal_matrix;
uniform Light u_light;

void main()
{
    vec4 world_position = u_model_matrix * vec4(in_position, 1.0);
    gl_Position = u_projection_matrix * u_view_matrix * world_position;


    _tex_coord = in_tex_coord;
    _normal = u_normal_matrix * in_normal;
    _world_position = world_position.xyz;
}
