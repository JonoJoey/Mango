#version 330 core

layout(location = 0) out vec4 out_color;

in vec4 _color;

void main()
{
    out_color = _color;
}
