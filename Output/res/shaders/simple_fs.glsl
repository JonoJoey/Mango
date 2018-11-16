#version 330 core

layout(location = 0) out vec4 out_color;

in vec2 _tex_coords;

uniform sampler2D u_texture;

void main()
{
    out_color = texture(u_texture, _tex_coords);
}
