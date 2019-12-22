#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec4 position;
in vec4 lightpos;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightpos.xyz - position.xyz);

    // Looking at back side flip normals.
    if (!gl_FrontFacing)
        norm = -norm;

    float diffuse = max(0.0, dot(norm,lightDir));
    FragColor = vec4(diffuse, diffuse, diffuse , 1.0);
}
