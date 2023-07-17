#include "partical.h"
#include <glm/glm/glm.hpp>		//use to use matrix and vector

using namespace glm;

pParticle Particles;
UINT nOfFires = 0;
UINT Tick1 = 0, Tick2 = 0;
float DTick = 0;
GLuint textureID = 0;

void AddParticle(Particle ex)
{
	pParticle p;
	p = new Particle;//new particle   
	p->pNext = NULL;
	p->pPrev = NULL;
	p->b = ex.b;
	p->g = ex.g;
	p->r = ex.r;
	p->fade = ex.fade;
	p->life = ex.life;
	p->size = ex.size;
	p->xpos = ex.xpos;
	p->ypos = ex.ypos;
	p->zpos = ex.zpos;
	p->gravity = ex.gravity;
	p->xspeed = ex.xspeed;
	p->yspeed = ex.yspeed;
	p->zspeed = ex.zspeed;
	p->AddCount = ex.AddCount;
	p->AddSpeed = ex.AddSpeed;
	p->bAddParts = ex.bAddParts;
	p->bFire = ex.bFire;
	p->nExpl = ex.nExpl;
	if (!Particles)//當目前的Particle列表為空時
	{
		Particles = p;
		return;
	}
	else
	{
		Particles->pPrev = p;//插入Particle   
		p->pNext = Particles;
		Particles = p;
	}
}

void DeleteParticle(pParticle* p)
{
	if (!(*p))//假如Particle列表為空
		return;
	if (!(*p)->pNext && !(*p)->pPrev)//假如只有一個Particle，直接删除  
	{
		delete(*p);
		*p = NULL;
		return;
	}
	pParticle tmp;
	if (!(*p)->pPrev)//假如是首節點  
	{
		tmp = (*p);
		*p = (*p)->pNext;
		Particles = *p;
		(*p)->pPrev = NULL;
		delete tmp;
		return;
	}
	if (!(*p)->pNext)//末節點  
	{
		(*p)->pPrev->pNext = NULL;
		delete (*p);
		*p = NULL;
		return;
	}
	else
	{
		//一般情况  
		tmp = (*p);
		(*p)->pPrev->pNext = (*p)->pNext;
		(*p)->pNext->pPrev = (*p)->pPrev;
		*p = (*p)->pNext;
		delete tmp;
	}
}

void DeleteAll(pParticle* Part)
{
	while ((*Part))
		DeleteParticle(Part);
}

void InitParticle(Particle& ep)
{
	ep.b = float(rand() % 100) / 60.0f;//顏色隨機
	ep.g = float(rand() % 100) / 60.0f;
	ep.r = float(rand() % 100) / 60.0f;
	ep.life = 1.0f;//初始壽命
	ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
	ep.size = 1;//大小  
	ep.xpos = 30.0f - float(rand() % 601) / 10.0f;//位置 
	ep.ypos = 0.0f;
	ep.zpos = 20.0f - float(rand() % 401) / 10.0f;
	if (!int(ep.xpos))//x方向速度(z方向相同)
	{
		ep.xspeed = 0.0f;
	}
	else
	{
		if (ep.xpos < 0)
		{
			ep.xspeed = (rand() % int(-ep.xpos)) / 1500.0f;
		}
		else
		{
			ep.xspeed = -(rand() % int(-ep.xpos)) / 1500.0f;
		}
	}
	ep.yspeed = 0.04f + float(rand() % 11) / 1000.0f;//y方向速度(向上)
	ep.bFire = 1;
	ep.nExpl = 1 + rand() % 6;//粒子效果  
	ep.bAddParts = 1;//設定有尾巴 
	ep.AddCount = 0.0f;
	ep.AddSpeed = 0.2f;
	nOfFires++;//粒子數+1 
	AddParticle(ep);//加入粒子列表    
}

void InitParticle_smoke(Particle& ep)
{
	for (int i = 0; i < 1000; i++)
	{
		//ep.b = float(rand() % 100) / 60.0f;//顏色隨機
		//ep.g = float(rand() % 100) / 60.0f;
		//ep.r = float(rand() % 100) / 60.0f;
		ep.b = 1.0f;//顏色隨機
		ep.g = 1.0f;
		ep.r = 1.0f;
		ep.life = 4.0f + float(rand() % 10) / 100.0f;//初始壽命
		ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
		ep.size = 1;//大小  
		ep.gravity = 0.00003f;
		ep.xpos = 0.0f + (float(rand() % 8000) / 10.0f) - 400.0f;//位置 
		ep.ypos = 100.0f + (float(rand() % 4000) / 10.0f);
		ep.zpos = 0.0f + (float(rand() % 8000) / 10.0f) - 400.0f;

		ep.zspeed = 0.0f;//float(rand() % 11) / 1000.0f;
		ep.xspeed = 0.0f;//float(rand() % 11) / 1000.0f;

		ep.yspeed = 0.0f; //+ float(rand() % 1000) / 1000.0f; //y方向速度(向上)
		ep.bFire = 1;
		ep.nExpl = 8; //粒子效果  
		ep.bAddParts = 0; //設定有尾巴 
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.2f;
		nOfFires++; //粒子數+1 
		AddParticle(ep); //加入粒子列表    
	}
}

void InitParticle_fountain(Particle& ep)
{
	for (int i = 0; i < 5000; i++)
	{
		//ep.b = float(rand() % 100) / 60.0f;//顏色隨機
		//ep.g = float(rand() % 100) / 60.0f;
		//ep.r = float(rand() % 100) / 60.0f;
		float temp = float(rand() % 50) / 100.0f;
		ep.b = 0.5f + temp;//顏色隨機
		ep.g = temp;
		ep.r = temp;
		ep.life = 0.4f + float(rand() % 10) / 100.0f;//初始壽命
		ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
		ep.size = 0.1f + (float(rand() % 10) / 100.0f);//大小  
		ep.gravity = 0.0003f;// + (float(rand() % 10) / 100000.0f);
		ep.xpos = 15.0f + (float(rand() % 40) / 100.0f);//位置 
		ep.ypos = 20.0f + (float(rand() % 40) / 100.0f);
		ep.zpos = -7.0f + (float(rand() % 40) / 100.0f);

		ep.zspeed = 0.0f + float(rand() % 50) / 1000.0f - 0.025;
		ep.xspeed = 0.0f + float(rand() % 50) / 1000.0f - 0.025;

		ep.yspeed = 0.15f + float(rand() % 10) / 1000.0f; //y方向速度(向上)
		ep.bFire = 1;
		ep.nExpl = 9; //粒子效果  
		ep.bAddParts = 0; //設定有尾巴 
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.2f;
		nOfFires++; //粒子數+1 
		AddParticle(ep); //加入粒子列表    
	}

}

void InitParticle_star(Particle& ep)
{
	for (int i = 0; i < 500; i++)
	{
		//ep.b = float(rand() % 100) / 60.0f;//顏色隨機
		//ep.g = float(rand() % 100) / 60.0f;
		//ep.r = float(rand() % 100) / 60.0f;
		float temp = float(rand() % 50) / 100.0f;
		ep.b = temp;//顏色隨機
		ep.g = temp;
		ep.r = temp;
		ep.life = 0.8;//0.4f + float(rand() % 10) / 100.0f;//初始壽命
		ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
		ep.size = 1 + (float(rand() % 10) / 100.0f);//大小  
		ep.gravity = 0.0000f;// + (float(rand() % 10) / 100000.0f);
		ep.xpos = 0.0f + (float(rand() % 4000) / 10.0f) - 200.0;//位置 
		ep.ypos = 100.0f;// +(float(rand() % 4000) / 10.0f) - 200.0;
		ep.zpos = 0.0f + (float(rand() % 4000) / 10.0f) - 200.0;

		ep.zspeed = 0.0f;//+ float(rand() % 50) / 1000.0f - 0.025;
		ep.xspeed = 0.0f; //+ float(rand() % 50) / 1000.0f - 0.025;

		ep.yspeed = 0.0f;//0.15f; +float(rand() % 10) / 1000.0f; //y方向速度(向上)
		ep.bFire = 1;
		ep.nExpl = 11; //粒子效果  
		ep.bAddParts = 0; //設定有尾巴 
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.2f;
		nOfFires++; //粒子數+1 
		AddParticle(ep); //加入粒子列表    
	}
}

void InitParticle_fire(Particle& ep, float x, float y, float z)
{
	for (int i = 0; i < 3000; i++)
	{
		//ep.b = float(rand() % 100) / 60.0f;//顏色隨機
		//ep.g = float(rand() % 100) / 60.0f;
		//ep.r = float(rand() % 100) / 60.0f;
		float temp = float(rand() % 50) / 100.0f;
		ep.b = 0;//temp;//顏色隨機
		ep.g = 0.5+temp;
		ep.r = 0.5f + temp;
		ep.life = 0.1f + float(rand() % 10) / 200.0f;//初始壽命
		ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
		ep.size = 0.1f + (float(rand() % 10) / 100.0f);//大小  
		ep.gravity = 0.0f;// + (float(rand() % 10) / 100000.0f);

		ep.xpos = x;//+ (float(rand() % 40) / 100.0f);//位置 
		ep.ypos = y;//+ (float(rand() % 40) / 100.0f);
		ep.zpos = z;//+ (float(rand() % 40) / 100.0f);

		ep.zspeed = 0.0f + float(rand() % 100) / 1000.0f - 0.05;
		ep.xspeed = 0.0f + float(rand() % 100) / 1000.0f - 0.05;

		ep.yspeed = 0.0f + float(rand() % 100) / 1000.0f - 0.05; //y方向速度(向上)
		ep.bFire = 1;
		ep.nExpl = 10; //粒子效果  
		ep.bAddParts = 0; //設定有尾巴 
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.2f;
		nOfFires++; //粒子數+1 
		AddParticle(ep); //加入粒子列表    
	}
}

void Explosion1(Particle* par)
{
	Particle ep;
	for (int i = 0; i < 100; i++)
	{
		ep.b = float(rand() % 100) / 60.0f;
		ep.g = float(rand() % 100) / 60.0f;
		ep.r = float(rand() % 100) / 60.0f;
		ep.life = 1.0f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.yspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.zspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion2(Particle* par)
{
	Particle ep;
	for (int i = 0; i < 1000; i++)
	{
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 1.0f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.yspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.zspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion3(Particle* par)
{
	Particle ep;
	float PIAsp = 3.1415926 / 180;
	for (int i = 0; i < 30; i++) {
		float angle = float(rand() % 360)*PIAsp;
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 1.5f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = (float)sin(angle)*0.01f;
		ep.yspeed = 0.01f + float(rand() % 11) / 1000.0f;
		ep.zspeed = (float)cos(angle)*0.01f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 1;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.2f;
		AddParticle(ep);
	}
}

void Explosion4(Particle* par) {
	Particle ep;
	float PIAsp = 3.1415926 / 180;
	for (int i = 0; i < 30; i++) {
		float angle = float(rand() % 360)*PIAsp;
		ep.b = float(rand() % 100) / 60.0f;
		ep.g = float(rand() % 100) / 60.0f;
		ep.r = float(rand() % 100) / 60.0f;
		ep.life = 1.5f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = (float)sin(angle)*0.01f;
		ep.yspeed = 0.01f + float(rand() % 11) / 1000.0f;
		ep.zspeed = (float)cos(angle)*0.01f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 1;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.2f;
		AddParticle(ep);
	}
}

void Explosion5(Particle* par) {
	Particle ep;
	for (int i = 0; i < 30; i++) {
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 0.8f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.yspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.zspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 7;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion6(Particle* par) {
	Particle ep;
	for (int i = 0; i < 100; i++) {
		ep.b = float(rand() % 100) / 60.0f;
		ep.g = float(rand() % 100) / 60.0f;
		ep.r = float(rand() % 100) / 60.0f;
		ep.life = 0.8f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.yspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.zspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 7;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion7(Particle* par) {
	Particle ep;
	for (int i = 0; i < 10; i++) {
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 0.5f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.6f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.yspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.zspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion8(Particle* par)
{
	Particle ep;
	ep.b = 1.0f;//顏色隨機
	ep.g = 1.0f;
	ep.r = 1.0f;
	ep.life = 4.0f + float(rand() % 10) / 100.0f;//初始壽命
	ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
	ep.size = 1;//大小  
	ep.gravity = (float(rand() % 6) / 100000.0f);
	ep.xpos = 0.0f + (float(rand() % 4000) / 10.0f) - 200.0f;//位置 
	ep.ypos = 100.0f + (float(rand() % 4000) / 10.0f);
	ep.zpos = 0.0f + (float(rand() % 4000) / 10.0f) - 200.0f;

	ep.zspeed = 0.0f;//float(rand() % 11) / 1000.0f;
	ep.xspeed = 0.0f;//float(rand() % 11) / 1000.0f;

	ep.yspeed = 0.0f; //+ float(rand() % 1000) / 1000.0f; //y方向速度(向上)
	ep.bFire = 1;
	ep.nExpl = 8; //粒子效果  
	ep.bAddParts = 0; //設定有尾巴 
	ep.AddCount = 0.0f;
	ep.AddSpeed = 0.2f;
	nOfFires++; //粒子數+1 
	AddParticle(ep); //加入粒子列表    
}

void Explosion9(Particle* par)
{
	Particle ep;
	float temp = float(rand() % 50) / 100.0f;
	ep.b = 0.5f + temp;//顏色隨機
	ep.g = temp;
	ep.r = temp;
	ep.life = 0.4f + float(rand() % 10) / 100.0f;//初始壽命
	ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
	ep.size = 0.1f + (float(rand() % 10) / 100.0f);//大小  
	ep.gravity = 0.0003f;// + (float(rand() % 10) / 100000.0f);

	ep.xpos = 15.0f + (float(rand() % 40) / 100.0f);//位置 
	ep.ypos = 20.0f + (float(rand() % 40) / 100.0f);
	ep.zpos = -7.0f + (float(rand() % 40) / 100.0f);

	ep.zspeed = 0.0f + float(rand() % 50) / 1000.0f - 0.025;
	ep.xspeed = 0.0f + float(rand() % 50) / 1000.0f - 0.025;

	ep.yspeed = 0.15f + float(rand() % 10) / 1000.0f; //y方向速度(向上)
	ep.bFire = 1;
	ep.nExpl = 9; //粒子效果  
	ep.bAddParts = 0; //設定有尾巴 
	ep.AddCount = 0.0f;
	ep.AddSpeed = 0.2f;
	nOfFires++; //粒子數+1 
	AddParticle(ep); //加入粒子列表    
}

void Explosion10(Particle* par, float x, float y, float z)
{
	Particle ep;
	float temp = float(rand() % 50) / 100.0f;
	ep.b = temp;//顏色隨機
	ep.g = 0.2+temp;
	ep.r = 0.5f + temp;
	ep.life = 0.1f + float(rand() % 10) / 200.0f;//初始壽命
	ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
	ep.size = 0.1f + (float(rand() % 10) / 100.0f);//大小  
	ep.gravity = 0.0f;// + (float(rand() % 10) / 100000.0f);

	ep.xpos = x;//+ (float(rand() % 40) / 100.0f);//位置 
	ep.ypos = y;//+ (float(rand() % 40) / 100.0f);
	ep.zpos = z;//+ (float(rand() % 40) / 100.0f);
	ep.zspeed = 0.0f + float(rand() % 100) / 1000.0f - 0.05;
	ep.xspeed = 0.0f + float(rand() % 100) / 1000.0f - 0.05;

	ep.yspeed = 0.0f + float(rand() % 100) / 1000.0f - 0.05; //y方向速度(向上)
	ep.bFire = 1;
	ep.nExpl = 10; //粒子效果  
	ep.bAddParts = 0; //設定有尾巴 
	ep.AddCount = 0.0f;
	ep.AddSpeed = 0.2f;
	nOfFires++; //粒子數+1 
	AddParticle(ep); //加入粒子列表    
}


void Explosion11(Particle* par)
{
	Particle ep;
	float temp = float(rand() % 50) / 100.0f;
	ep.b = temp;//顏色隨機
	ep.g = temp;
	ep.r = temp;
	ep.life = 0.8;//0.4f + float(rand() % 10) / 100.0f;//初始壽命
	ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
	ep.size = 1 + (float(rand() % 10) / 100.0f);//大小  
	ep.gravity = 0.0000f;// + (float(rand() % 10) / 100000.0f);
	ep.xpos = 0.0f + (float(rand() % 4000) / 10.0f) - 200.0;//位置 
	ep.ypos = 100.0f;// +(float(rand() % 4000) / 10.0f) - 200.0;
	ep.zpos = 0.0f +(float(rand() % 4000) / 10.0f) - 200.0;

	ep.zspeed = 0.0f;//+ float(rand() % 50) / 1000.0f - 0.025;
	ep.xspeed = 0.0f; //+ float(rand() % 50) / 1000.0f - 0.025;

	ep.yspeed = 0.0f;//0.15f; +float(rand() % 10) / 1000.0f; //y方向速度(向上)
	ep.bFire = 1;
	ep.nExpl = 11; //粒子效果  
	ep.bAddParts = 0; //設定有尾巴 
	ep.AddCount = 0.0f;
	ep.AddSpeed = 0.2f;
	nOfFires++; //粒子數+1 
	AddParticle(ep); //加入粒子列表    
}



void DrawParticles(void)
{
	glBindTexture(GL_TEXTURE_2D, 1);
	pParticle par;
	par = Particles;
	while (par)
	{
		glColor4f(par->r, par->g, par->b, par->life);
		mat4 matrix;
		GLfloat looker[16] = { 0 };
		glGetFloatv(GL_MODELVIEW_MATRIX, looker);


		
		//glRotated(-45, 0, 1, 0);

		float x;
		float y;
		float z;
		float tmpx;
		float tmpy;
		float tmpz;


		

		glBegin(GL_TRIANGLE_STRIP);
glPushMatrix();
x = par->size;
		y = par->size;
		z = 0;
		tmpx = x * looker[0] + y * looker[1] + z * looker[2];
		tmpy = x * looker[4] + y * looker[5] + z * looker[6];
		tmpz = x * looker[8] + y * looker[9] + z * looker[10];

		x = par->xpos + tmpx;
		y = par->ypos + tmpy;
		z = par->zpos + tmpz;





		glTexCoord2d(1, 1);
		glVertex3f(x, y, z);


		x = -par->size;
		y = par->size;
		z = 0;
		tmpx = x * looker[0] + y * looker[1] + z * looker[2];
		tmpy = x * looker[4] + y * looker[5] + z * looker[6];
		tmpz = x * looker[8] + y * looker[9] + z * looker[10];
		x = par->xpos + tmpx;
		y = par->ypos + tmpy;
		z = par->zpos + tmpz;
		glTexCoord2d(0, 1);
		glVertex3f(x, y, z);

		x = par->size;
		y = -par->size;
		z = 0;
		tmpx = x * looker[0] + y * looker[1] + z * looker[2];
		tmpy = x * looker[4] + y * looker[5] + z * looker[6];
		tmpz = x * looker[8] + y * looker[9] + z * looker[10];
		x = par->xpos + tmpx;
		y = par->ypos + tmpy;
		z = par->zpos + tmpz;
		glTexCoord2d(1, 0);
		glVertex3f(x, y, z);

		x = -par->size;
		y = -par->size;
		z = 0;
		tmpx = x * looker[0] + y * looker[1] + z * looker[2];
		tmpy = x * looker[4] + y * looker[5] + z * looker[6];
		tmpz = x * looker[8] + y * looker[9] + z * looker[10];
		x = par->xpos + tmpx;
		y = par->ypos + tmpy;
		z = par->zpos + tmpz;
		glTexCoord2d(0, 0);
		glVertex3f(x, y, z);
glPopMatrix();

		glEnd();


		
		par = par->pNext;
	}
}
