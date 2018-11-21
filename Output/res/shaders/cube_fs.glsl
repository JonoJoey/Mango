#version 330 core

layout (location = 0) out vec4 out_color;

in vec3 _normal;
flat in int _block_type;

uniform samplerCube u_textures[16];

void main()
{
    if (_block_type == 0)
        discard;

    vec4 color = texture(u_textures[_block_type - 1], _normal);
    out_color = color;
}
