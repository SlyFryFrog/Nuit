#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 uLightPos;

void main()
{
    // Apply Lambertian reflectance
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);

    // Calculate the cosine of the angle between the normal and light direction
    float cosTheta = max(dot(norm, lightDir), 0.0);

    // Compute the diffuse reflection
    vec3 diffuse = cosTheta * vec3(0.8, 0.8, 0.8);

    FragColor = vec4(diffuse, 1.0);
}
