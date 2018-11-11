#version 330 core

layout(location = 0) out vec4 out_color;

uniform sampler2D u_texture;

in vec2 _tex_coord;

void main()
{
    out_color = texture(u_texture, _tex_coord);
}
