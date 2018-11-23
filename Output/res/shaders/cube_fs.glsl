#version 330 core

layout (location = 0) out vec4 out_color;

in vec3 _normal;

void main()
{
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    out_color = color;
}
