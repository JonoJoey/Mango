#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in int in_block_type;

out vec3 _normal;
flat out int _block_type;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

void main()
{
    int x = gl_InstanceID % 16;
    int y = (gl_InstanceID / 16) % 256;
    int z = gl_InstanceID / (16 * 256);

    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(in_position + vec3(x, y, -z), 1.0);

    _normal = in_position;
    _block_type = in_block_type;
}
