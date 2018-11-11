#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_tex_coord;

out vec2 _tex_coord;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

void main()
{
    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(in_position, 1.0);

    _tex_coord = in_tex_coord;
}
