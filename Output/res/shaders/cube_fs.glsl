#version 330 core

layout (location = 0) out vec4 out_color;

in vec3 _normal;

uniform sampler2D u_texture;

void main()
{

    vec4 color = texture(u_texture, gl_FragCoord.xy / vec2(800.0, 600.0)); // texture(u_texture, _normal);
    out_color = color;
}
