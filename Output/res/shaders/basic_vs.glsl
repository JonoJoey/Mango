#version 330 core

layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec4 in_Color;

out vec4 _Color;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(in_Position, 0.0, 1.0);
    _Color = in_Color;
}
