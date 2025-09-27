#version 410 core

layout(location = 0) in vec2 aPos;

uniform float uRadius;

void main()
{
    vec2 pos = uRadius * aPos;
    gl_Position = vec4(pos, 0.0, 1.0);
}