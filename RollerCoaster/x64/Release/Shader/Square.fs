#version 430 core

in vec3 vUV;

uniform samplerCube Texture;

out vec4 fColor;

void main()
{ 
 fColor = texture(Texture,vUV);
}