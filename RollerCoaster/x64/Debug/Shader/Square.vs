#version 430 core

layout(location = 0) in vec3 vertex;


uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

out vec3 vUV;

void main(void)
{
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(vertex, 1.0);
    vUV = vertex;
}
