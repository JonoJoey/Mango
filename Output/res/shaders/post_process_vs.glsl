#version 330 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_coords;

out vec2 _tex_coords;

void main()
{
    gl_Position = vec4(in_position, 0.0, 1.0);
    _tex_coords = in_tex_coords;
}
