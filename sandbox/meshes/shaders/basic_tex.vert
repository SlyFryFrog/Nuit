#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
out vec2 TexCoord;

void main()
{
    // Convert Model Position to World Position
    FragPos = vec3(uModel * vec4(aPos, 1.0));

    // Remove translation components by converting to mat3
    // Then converto Model Normal to World Normal
    Normal = mat3(transpose(inverse(uModel))) * aNormal;

    // Convert from World Space to Clip Space
    gl_Position = uProjection * uView * vec4(FragPos, 1.0);
    TexCoord = aTex;
}
