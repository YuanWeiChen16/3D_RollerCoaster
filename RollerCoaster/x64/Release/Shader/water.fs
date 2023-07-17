#version 430 core
varying vec3 position;
varying vec3 worldNormal;
varying vec3 eyeNormal;
uniform vec3 eyePos;
uniform mat4 model_matrixs;
uniform samplerCube envMap;

uniform vec4 color_ambient = vec4(0.1,0.2,0.5,1.0);
uniform vec4 color_diffuse = vec4(0.2,0.3,0.6,1.0);
uniform vec4 color_specular = vec4(1.0,1.0,1.0,1.0);
uniform vec4 Color = vec4(0.1,0.1,0.1,1.0);
uniform float shininess = 77.0f;
uniform vec3 light_position = vec3(50.0f,32.0f,560.0f);

layout (location = 0 ) out vec4 colors;

vec3 ExtractCameraPos(const mat4 a_modelView)
{
  // Get the 3 basis vector planes at the camera origin and transform them into model space.
  //  
  // NOTE: Planes have to be transformed by the inverse transpose of a matrix
  //       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
  //
  //       So for a transform to model space we need to do:
  //            inverse(transpose(inverse(MV)))
  //       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
  //
  // As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
  //  
  mat4 modelViewT = transpose(a_modelView);
 
  // Get plane normals 
  vec3 n1 = vec3(modelViewT[0]);
  vec3 n2 = vec3(modelViewT[1]);
  vec3 n3 = vec3(modelViewT[2]);
 
  // Get plane distances
  float d1 =(modelViewT[0].w);
  float d2 =(modelViewT[1].w);
  float d3 =(modelViewT[2].w);
 
  // Get the intersection of these 3 planes
  // http://paulbourke.net/geometry/3planes/
  vec3 n2n3 = cross(n2, n3);
  vec3 n3n1 = cross(n3, n1);
  vec3 n1n2 = cross(n1, n2);
 
  vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
  float denom = dot(n1, n2n3);
 
  return top / -denom;
}
void main() {
    vec3 eye = normalize(ExtractCameraPos(model_matrixs) - vec3(position.x,position.y,position.z));
    //vec3 eye = normalize(eyePos - position);
    vec3 r = reflect(vec3(-eye.x,eye.y,eye.z), worldNormal);
    vec3 r1 = refract(vec3(-eye.x,-eye.y,eye.z),worldNormal,0.66);
    vec4 color = mix( textureCube(envMap, r),textureCube(envMap, r1),0.5);
    //vec4 color = textureCube(envMap, r1);
    //vec4 color = textureCube(envMap, r);
    color.a = 1;
    //gl_FragColor = color;
    //colors = vec4(0.0,0.0,1.0,1.0);
    
   /* vec3 light_direction =  normalize(light_position-position);
	vec3 normal = normalize(worldNormal);
	vec3 half_vector = normalize(normalize(light_direction)+normalize(vec3(-eye.x,eye.y,eye.z)));
	float diffuse = max(0.0,dot(normal,light_direction));
	float specular =  pow(max(0.0,dot(worldNormal,half_vector)),shininess);
	color =mix(color, min(Color * color_ambient,vec4(1.0))+diffuse * color_diffuse + specular * color_specular,0.5);*/
    colors = color;
}


