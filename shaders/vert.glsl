#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec4 position;
out vec4 lightpos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
    position = view * vec4(aPos, 1.0);
    lightpos = view * vec4(1.0f, 1.0f, 0.0f, 1.0);
    normal   = vec3( view * vec4(aNormal, 0.0));

    gl_Position = projection * position;
}
