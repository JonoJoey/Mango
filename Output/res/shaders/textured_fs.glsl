#version 330 core

layout(location = 0) out vec4 out_Color;

in vec2 _TexCoord;

uniform sampler2D u_Texture;

void main()
{
    out_Color = texture(u_Texture, _TexCoord);
}
