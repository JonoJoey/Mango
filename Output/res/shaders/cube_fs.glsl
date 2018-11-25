#version 330 core

layout (location = 0) out vec4 out_color;

in vec3 _normal;
in vec2 _tex_coords;
flat in int _block_id;

uniform sampler2DArray u_texture;

void main()
{
    vec4 color = texture(u_texture, vec3(_tex_coords, _block_id));
    out_color = color;
}
