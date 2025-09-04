#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 uLightPos;
uniform vec3 uKd;   // Fallback diffuse color
uniform sampler2D uDiffuseTex;

void main()
{
    // Apply Lambertian reflectance
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);

    // Calculate the cosine of the angle between the normal and light direction
    float cosTheta = max(dot(norm, lightDir), 0.0);

    // Compute the diffuse reflection
    vec3 diffuse = cosTheta * vec3(0.8, 0.8, 0.8);

    // Apply our diffuse to our texture
    FragColor = vec4(diffuse, 1.0) * texture(uDiffuseTex, TexCoord);
}
