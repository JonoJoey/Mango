#version 330 core

layout(location = 0) out vec4 out_color;

in vec2 _tex_coords;

uniform sampler2D u_texture;

void main()
{
    vec3 color = texture(u_texture, _tex_coords).rgb;

    // gamma correction
    float gamma = 2.2;
    color = pow(color, vec3(1.0 / gamma));

    out_color.rgb = color;
    out_color.a = 1.0;
}
