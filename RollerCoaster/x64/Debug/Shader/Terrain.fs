#version 330 core

layout (location = 0 )out vec4 fcolor;

in vec3 vs_normal;
in vec3 vs_worldpos;
in vec3 modeview;
in vec3 eyeNormal;
in vec3 apos;
in vec2 texcood;

uniform samplerCube Texture0;
uniform sampler2D Texture3;

uniform float shiness = 128.0f;
uniform vec3 light_position = vec3(50.0f,32.0f,560.0f);
uniform vec3 EyeDirection = vec3(0.0,0.0,1.0);

void main(void)
{

vec3 light_direction = normalize(light_position - vs_worldpos);
vec3 normal=normalize(vs_normal);
vec3 half_vector = normalize(normalize(light_direction) + normalize(EyeDirection));
float diffuse =max(0.0,dot(normal,light_direction));
float specular =pow( max(0.0 , dot(vs_normal , half_vector)) , shiness);


/*
 vec3 eye = normalize(vs_worldpos - modeview);
 //eye = vec3(eye.x,eye.z,eye.y);
 vec3 r = reflect(eye, normalize(eyeNormal));
 vec3 r1 = refract(eye, normalize(eyeNormal),0.66);
 //vec4 Color = mix( textureCube(Texture0, r),textureCube(Texture0, r1),0.5);
 vec4 Color = textureCube(Texture0, r);
*/
 
 
/*
vec3 temp = vec3(reflect(apos,texcood));
temp = vec3(temp[0],temp[1],temp[2]);
vec4 Color = texture(Texture0,temp);
*/


//vec4 Color = vec4(1.0,0.0,0.0,1.0);

vec4 Color = texture2D(Texture3,texcood);


vec4 color_ambient = Color*0.75;
vec4 color_diffude = Color;
vec4 color_specular = vec4(1.0,1.0,1.0,1.0); 

/*
vec4 basecolor = tex2D(WaterTex,gl_TexCoord[1].xy);
 vec3 ReflectColor = vec3(textureCube(WaterCube, ReflectVec));
   vec3 RefractColor = vec3(textureCube(WaterCube, RefractVec)); 
 
   vec3 outputCol = mix(RefractColor, ReflectColor, Ratio); 
 
   gl_FragColor = vec4( outputCol * basecolor , 1.0 );
*/

fcolor = min(Color*color_ambient,vec4(1.0)) + diffuse*  color_diffude + specular * color_specular;
//fcolor =vec4(Color[0],Color[1],Color[2],1.0);
fcolor.a=1.0;
}