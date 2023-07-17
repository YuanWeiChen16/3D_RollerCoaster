#pragma once
#include <QGLWidget> 
#include <QtGui>  
#include <QtOpenGL>  
#include <GL/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib") 
#include "Utilities/ArcBallCam.H"
#include "Utilities/3DUtils.H"

#define MAX_PARTICLES 10000 
#define MAX_FIRES 5  



typedef struct tag_PARTICLE
{
	GLfloat xpos;
	GLfloat ypos;
	GLfloat zpos;
	GLfloat xspeed;//(xspeed,yspeed,zspeed)為particle的speed 
	GLfloat yspeed;
	GLfloat zspeed;
	GLfloat r;//(r,g,b)為particle的color
	GLfloat g;
	GLfloat b;
	GLfloat gravity;
	GLfloat life;// particle的壽命 
	GLfloat fade;// particle的衰減速度
	GLfloat size;// particle的大小  
	GLbyte bFire;
	GLbyte nExpl;//哪種particle效果  
	GLbyte bAddParts;// particle是否含有尾巴
	GLfloat AddSpeed;//尾巴粒子的加速度  
	GLfloat AddCount;//尾巴粒子的增加量  
	tag_PARTICLE* pNext;//下一particle 
	tag_PARTICLE* pPrev;//上一particle   
} Particle, *pParticle;

void AddParticle(Particle ex);
void DeleteParticle(pParticle* p);
void DeleteAll(pParticle* Part);
void InitParticle(Particle& ep);
void InitParticle_smoke(Particle& ep);
void InitParticle_fountain(Particle& ep);
void InitParticle_star(Particle& ep);
void InitParticle_fire(Particle& ep, float x, float y, float z);
void Explosion1(Particle* par);
void Explosion2(Particle* par);
void Explosion3(Particle* par);
void Explosion4(Particle* par);
void Explosion5(Particle* par);
void Explosion6(Particle* par);
void Explosion7(Particle* par);
void Explosion8(Particle* par);
void Explosion9(Particle* par);
void Explosion10(Particle* par, float x, float y, float z);
void Explosion11(Particle* par);
void DrawParticles(void);
