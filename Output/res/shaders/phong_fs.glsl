#version 330 core

layout(location = 0) out vec4 out_color;

uniform sampler2D u_texture;

in vec2 _tex_coord;

void main()
{
    vec4 color = texture(u_texture, _tex_coord);
    out_color = color;
}
