#version 330 core

layout(location = 0) in vec3 vertex;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform float times;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
out vec3 vs_worldpos;
out vec3 vs_normal;
out vec3 eyeNormal;
out vec3 apos;
out vec3 modeview;
out vec2 texcood;
void main(void)
{
	vec3 temp = (vertex.xyz + vec3(800 , 800 , 800 + times)) / 1600;
	//vec3 temp = (vertex.xyz + vec3(80 , 80 , 80 )) / 160;
	vec4 height = texture2D(Texture1,vec2(temp[0],temp[2]));
	vec4 normal = texture2D(Texture2,vec2(temp[0],temp[2]));
	vec3 mapp = vec3(0,(height.r +height.g + height.b)*30.0-45.0,0);
	
	vec3 nor = vec3((normal.r*2.0 - 1.0) , (normal.b*2.0 - 1.0) , (normal.g*2.0 - 1.0));
    
	gl_Position = (ProjectionMatrix * ModelViewMatrix * vec4((vertex + mapp), 1.0));
	//gl_Position = (ProjectionMatrix * ModelViewMatrix * vec4(vertex, 1.0));
	
	vs_normal = mat3(ModelViewMatrix) * (nor);
	//vs_normal = mat3(ModelViewMatrix) * vec3(0.0,1.0,0.0);
	
	vs_worldpos = gl_Position.xyz;
	modeview = vec3(ModelViewMatrix * vec4(0.0,0.0,20.0,1.0));
	
	//eyeNormal =  mat3(transpose(inverse(ModelViewMatrix))) * vec3(0.0,1.0,0.0);
	eyeNormal =  mat3(transpose(inverse(ModelViewMatrix))) * vec3(0.0,1.0,0.0);
	
	texcood = vec2(temp[0],temp[2]);
	//apos= vec3(ModelViewMatrix * vec4(vertex,1.0));
}
