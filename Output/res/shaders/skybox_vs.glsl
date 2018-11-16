#version 330 core

layout(location = 0) in vec3 in_position;

out vec3 _tex_coords;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;

void main()
{
    _tex_coords = in_position;
    vec4 pos = u_projection_matrix * u_view_matrix * vec4(in_position, 1.0);
    pos.z = pos.w * 0.999;
    gl_Position = pos;
}
