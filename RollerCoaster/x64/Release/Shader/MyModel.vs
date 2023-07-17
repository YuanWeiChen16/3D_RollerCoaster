#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MoveMatrix;
uniform vec3 eyepoiz=vec3(0.0,0.0,1.0);

uniform vec3 light_dir;

out vec3 halfff;
out vec3 vs_normal;
out vec3 BaryPos;
out vec3 tolight;
out vec2 vs_uv;

const vec3 BaryCoordPos[] = vec3[3]( vec3(1,0,0), vec3(0,1,0), vec3(0,0,1));
void main(void)
{
	mat4 MMatrix=transpose(MoveMatrix);
	vec4 pos_vs = vec4(MMatrix*vec4(position,1.0));
	halfff = normalize(normalize(light_dir)+normalize(eyepoiz));
	pos_vs= ModelViewMatrix *pos_vs;
	vec3 temp;
	temp=(ModelViewMatrix*vec4(light_dir,1.0)).xyz;
	tolight= normalize(temp - pos_vs.xyz);
	vs_normal = normalize((ModelViewMatrix*vec4(normal,0.0)).xyz);
	
	BaryPos = BaryCoordPos[gl_VertexID % 3];
	
	gl_Position = ProjectionMatrix * pos_vs;
	vs_uv = uv;
}