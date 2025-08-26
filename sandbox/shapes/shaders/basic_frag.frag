#version 410 core

uniform vec4 uColor;

out vec4 FragColor;

void main()
{
    if (length(uColor) > 0.0)
    {
        FragColor = uColor;
    }
    else
    {
        FragColor = vec4(1.0);
    }
}
