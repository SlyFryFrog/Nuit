#version 410 core

layout(location = 0) in vec2 aPos;

uniform float uOffset;
uniform float uRadius;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    vec2 pos = aPos * uRadius + uOffset;

    gl_Position = vec4(pos, 0.0, 1.0);
}