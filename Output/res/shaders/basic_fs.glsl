#version 330 core

layout(location = 0) out vec4 out_Color;

in vec4 _Color;

void main()
{
    out_Color = _Color;
    if (out_Color.a < 0.01) {
      discard;
    }
}
