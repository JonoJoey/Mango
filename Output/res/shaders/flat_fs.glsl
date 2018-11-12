#version 330 core

layout(location = 0) out vec4 out_color;

uniform vec3 flat_color;

void main()
{
    vec4 color = vec4(flat_color, 1.0);
    out_color = color;
}
