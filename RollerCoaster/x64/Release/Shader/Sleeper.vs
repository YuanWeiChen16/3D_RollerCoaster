#version 430 core

in vec3 vertex; // 第x段

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 G;
uniform vec3 controlP1;
uniform vec3 controlP2;
uniform vec3 controlP3;
uniform vec3 controlP4;
uniform vec3 orientP1;
uniform vec3 orientP2;
uniform vec3 orientP3;
uniform vec3 orientP4;
uniform int curveType;
uniform float ratio; 
uniform float ratio2;

vec3 Cross1(vec3 a,vec3 b) 
//=============================================================================
{
	vec3 q;
	q.x = b.z * a.y - b.y * a.z;
	q.y = b.x * a.z - b.z * a.x;
	q.z = b.y * a.x - b.x * a.y;
	return q;
};
vec3 normalize1(vec3 a)
//=============================================================================
{
	float l = a.x*a.x + a.y*a.y + a.z*a.z;
	if (l<0.000001) {
		a.x = 0;
		a.y = 1;
		a.z = 0;
	}
	else {
		l = sqrt(l);
		a.x /= l;
		a.y /= l;
		a.z /= l;
	}
    return a;
}
void main(void)
{
    
    float t = ratio;
    float t1 = ratio2;
    vec3 q0,q1,orient,final,cross1;
    mat4 G,G1,M,M1,T,T1;
    G = mat4(controlP1.x, controlP2.x, controlP3.x, controlP4.x,
			controlP1.y, controlP2.y, controlP3.y, controlP4.y,
			controlP1.z, controlP2.z, controlP3.z, controlP4.z,
			0.0f, 0.0f, 0.0f, 0.0f);
    G1 = mat4(orientP1.x, orientP2.x, orientP3.x, orientP4.x,
			orientP1.y, orientP2.y, orientP3.y, orientP4.y,
			orientP1.z, orientP2.z, orientP3.z, orientP4.z,
					0.0f, 0.0f, 0.0f, 0.0f);
	M = mat4(-1.0f, 2.0f, -1.0f, 0.0f,
		    3.0f, -5.0f, 0.0f, 2.0f,
			-3.0f, 4.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f, 0.0f);
	M *= 0.5;
    M1 = mat4(-1.0f, 3.0f, -3.0f, 1.0f,
            3.0f, -6.0f, 0.0f, 4.0f,
            -3.0f, 3.0f, 3.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 0.0f);
    M1 /= 6.0f;
	T = mat4(t*t*t, 0.0f, 0.0f, 0.0f,
			t*t, 0.0f, 0.0f, 0.0f,
			t, 0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 0.0f);
    T1 = mat4(t1*t1*t1, 0.0f, 0.0f, 0.0f,
            t1*t1, 0.0f, 0.0f, 0.0f,
            t1, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f);

    if(curveType  == 0)
    {
      q0  = (1.0f  -t ) *controlP1+ t *controlP2;
      q1 = (1.0f - t1 ) *controlP1+ t1 *controlP2;
      orient = (1.0f  -t ) *orientP1+ t *orientP2;
    }
    else if (curveType == 1)
    {
        mat4 QT = T * M * G;
        mat4 QT1 = T1 * M * G;
		mat4 O = T * M * G1;
        q0.x = QT[0][0];
        q0.y = QT[1][0];
        q0.z = QT[2][0];
        q1.x = QT1[0][0];
        q1.y = QT1[1][0];
        q1.z = QT1[2][0];
        orient.x = O[0][0];
        orient.y = O[1][0];
        orient.z = O[2][0];
    }
    else if(curveType == 2)
    {
        mat4 QT = T * M1* G;
        mat4 QT1 = T1 * M1 * G;
		mat4 O = T * M1 * G1;
        q0.x = QT[0][0];
        q0.y = QT[1][0];
        q0.z = QT[2][0];
        q1.x = QT1[0][0];
        q1.y = QT1[1][0];
        q1.z = QT1[2][0];
        orient.x = O[0][0];
        orient.y = O[1][0];
        orient.z = O[2][0];
    }
    cross1 = normalize1(cross1);
    orient = normalize1(orient);
    cross1 = Cross1((q1-q0),orient);
    cross1 = normalize1(cross1);
    cross1 = cross1 * 2.5;
    if(vertex.x == 0 )
    {
        final = q0+cross1;
    }
    else if(vertex.x == 1 )
    {
        final =  q1+cross1;
        
    }
    else if(vertex.x == 2 )
    {
        final = q0-orient;
    }
    else if(vertex.x == 3 )
    {
       final =  q1-orient;
    }
    else if(vertex.x  ==4)
    {
        final =  q0-cross1;
        
    }else if(vertex.x ==5)
    {
        final =  q1-cross1;
    }

    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(final, 1.0);
    //gl_Position =  vec4(vertex, 1.0);
}
