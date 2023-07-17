#include "TrainView.h"  
#include<iostream>

using namespace std;
using namespace glm;


TrainView::TrainView(QWidget *parent) :
	QGLWidget(parent)
{
	resetArcball();

}

TrainView::~TrainView()
{}
void TrainView::initializeGL()
{

	initializeOpenGLFunctions();
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER,0);
	//Create a triangle object
	triangle = new Triangle();
	//Initialize the triangle object
	triangle->Init();
	//Create a square object
	square = new Square();
	//Initialize the square object
	square->Init();
	//Initialize texture 
	initializeTexture();
	fbo = new FBO();
	fbo->Init();
	QMediaPlayer* player;
	water = new Water;
	terrain = new Terrain;
	terrain->Init();
	bump = new Bump;
	bump->Init();
	water->Init();
	lineTest = new TrackTest;
	lineTest->Init(DIVIDE_LINE, m_pTrack);
	sleeper = new Sleeper;
	sleeper->Init(DIVIDE_LINE, m_pTrack);
	tunnel = new Tunnel;
	tunnel->Init(DIVIDE_LINE, m_pTrack);
	player = new QMediaPlayer;
	player->setMedia(QUrl(QUrl::fromLocalFile("Eli Way - Wave.wav [Premiere].wav")));
	player->setVolume(30);
	//player->play();
	m = new Model("C:\\Users\\user\\Desktop\\rollerCoasterCar.obj", 15, Point3d(0, 0, 0));
	leftWheel = new Model("C:\\Users\\user\\Desktop\\LeftWheel.obj", 5, Point3d(0, 0, 0));
	rightWheel = new Model("C:\\Users\\user\\Desktop\\RightWheel.obj", 5, Point3d(0, 0, 0));
	dummy_sit = new Model("C:\\Users\\user\\Desktop\\dummy_noHand.obj", 3.5, Point3d(0, 0, 0));
	dummy_hand = new Model("C:\\Users\\user\\Desktop\\dummy_hand.obj", 3.5, Point3d(0, -0.1, 0.95));
	mineCraftCar = new Model("./Model/MinecraftCar.obj", 15, Point3d(0, 0, 0));


	dummy = new MyModel();
	dummy->Init("./Model/dummy.obj","./Model/dummy_wood.jpg",vec3(0,0,0),0.10);

	RCcar = new MyModel();
	RCcar->Init("./Model/rollerCoasterCar.obj", "./Model/metal.jpg", vec3(0, 0, 0), 1.0);
	gazebo = new MyModel();
	gazebo->Init("./Model/10073_Gazebo_V2_L3.obj", "./Model/10073_Gazebo_V2_Diffuse.jpg", vec3(0, 0, 0), 0.05);
	tree = new MyModel();
	tree->Init("./Model/MapleTreeStem.obj", "./Model/maple_bark.png", vec3(25, 0, -50), 1);
	tree1 = new MyModel();
	tree1->Init("./Model/MapleTreeLeaves.obj", "./Model/maple_leaf.png", vec3(25, 0, -50), 1);
	//android = new MyModel();
	//android->Init("./Model/android.obj", "./Model/green.png", vec3(10, 0, 0), 1.0);

	QMediaPlayer* musi;
	musi = new QMediaPlayer;
	musi->setMedia(QUrl(QUrl::fromLocalFile("music.wav")));
	musi->setVolume(50);
	musi->play();
}
void TrainView::initializeTexture()
{
	//Load and create a texture for square;'stexture
	QOpenGLTexture* texture;
	/*const  QImage posx = QImage("./Textures/ct_RT.png").convertToFormat(QImage::Format_RGBA8888);
	const   QImage posy = QImage("./Textures/ct_UP.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage posz = QImage("./Textures/ct_FR.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage negx = QImage("./Textures/ct_LF.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage negy = QImage("./Textures/ct_DN.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage negz = QImage("./Textures/ct_BK.png").convertToFormat(QImage::Format_RGBA8888);*/
	//Snow2048_rt Snow2048_bk
	const  QImage posx = QImage("./Textures/Snow2048_ft.png").convertToFormat(QImage::Format_RGBA8888);
	const   QImage posy = QImage("./Textures/Snow2048_up.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage posz = QImage("./Textures/Snow2048_rt.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage negx = QImage("./Textures/Snow2048_bk.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage negy = QImage("./Textures/Snow2048_dn.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage negz = QImage("./Textures/Snow2048_lf.png").convertToFormat(QImage::Format_RGBA8888);

	/*const  QImage posx = QImage("./Textures/posx.png").convertToFormat(QImage::Format_RGBA8888);
	const   QImage posy = QImage("./Textures/posy.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage posz = QImage("./Textures/posz.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage negx = QImage("./Textures/negx.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage negy = QImage("./Textures/negy.png").convertToFormat(QImage::Format_RGBA8888);
	const  QImage negz = QImage("./Textures/negz.png").convertToFormat(QImage::Format_RGBA8888);*/

	texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
	texture->create();
	texture->bind();
	texture->setSize(posx.width(), posx.height(), posx.depth());
	texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
	texture->allocateStorage();

	texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*)posx.constBits());
	texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*)posy.constBits());
	texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*)posz.constBits());
	texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*)negx.constBits());
	texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*)negy.constBits());
	texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*)negz.constBits());

	texture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
	Textures.push_back(texture);
	QOpenGLTexture* texture1 = new QOpenGLTexture(QImage("./Textures/terrain2.png"));
	Textures.push_back(texture1);

	QOpenGLTexture* texture2 = new QOpenGLTexture(QImage("./Textures/NormalMap.png"));
	Textures.push_back(texture2);

	QOpenGLTexture* texture3 = new QOpenGLTexture(QImage("./Textures/Ground1.png"));
	Textures.push_back(texture3);

	QOpenGLTexture* texture4 = new QOpenGLTexture(QImage("./Textures/bumpNormal.png"));
	Textures.push_back(texture4);
	QOpenGLTexture* texture5 = new QOpenGLTexture(QImage("./Textures/bumpTexture.png"));
	Textures.push_back(texture5);

}

void TrainView::resetArcball()
//========================================================================
{
	// Set up the camera to look at the world
	// these parameters might seem magical, and they kindof are
	// a little trial and error goes a long way
	arcball.setup(this, 40, 250, .2f, .4f, 0);
}
static unsigned long lastRedraw = 0;
void TrainView::paintGL()
{

	//*********************************************************************
	//
	// * Set up basic opengl informaiton
	//
	//**********************************************************************
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->FramebufferName);


	glBindTexture(GL_TEXTURE_2D, fbo->renderedTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 978, 486, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo->renderedTexture, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->FramebufferName);

	glBindRenderbuffer(GL_RENDERBUFFER, fbo->depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 978, 486);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->depthrenderbuffer);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Set up the view port
	glViewport(0, 0, width(), height());
	// clear the window, be sure to clear the Z-Buffer too
	glClearColor(0, 0, 0.3f, 0);

	// we need to clear out the stencil buffer since we'll use
	// it for shadows
	glClearStencil(0);
	glEnable(GL_DEPTH);

	// Blayne prefers GL_DIFFUSE
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// prepare for projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection();		// put the code to set up matrices here
	//m_3DS.Draw();
	//######################################################################
	// TODO: 
	// you might want to set the lighting up differently. if you do, 
	// we need to set up the lights AFTER setting up the projection
	//######################################################################
	// enable the lighting

	//====================================Train Run====================================
	if (isrun) {
		if (clock() - lastRedraw > CLOCKS_PER_SEC / 120) {
			/*lastRedraw = clock();
			float speed = 5.0f / 60.0;
			t_time += (speed / m_pTrack->points.size() / (DIVIDE_LINE / 40.0));
			if (t_time > 1.0f)
				t_time -= 1.0f;*/
			lastRedraw = clock();
			t_time += 1.0f / 120.0f;

		}
	}
	//=================================================================================

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// top view only needs one light
	if (this->camera == 1) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	}
	else {
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}

	//*********************************************************************
	//
	// * set the light parameters
	//
	//**********************************************************************
	GLfloat lightPosition1[] = { 0,1,1,0 }; // {50, 200.0, 50, 1.0};
	GLfloat lightPosition2[] = { 1, 0, 0, 0 };
	GLfloat lightPosition3[] = { 0, -1, 0, 0 };
	GLfloat yellowLight[] = { 0.5f, 0.5f, .1f, 1.0 };
	GLfloat whiteLight[] = { 1.0f, 1.0f, 1.0f, 1.0 };
	GLfloat blueLight[] = { .1f,.1f,.3f,1.0 };
	GLfloat grayLight[] = { .3f, .3f, .3f, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grayLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);

	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blueLight);
	//Get modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelViewMatrex);
	//Get projection matrix
	glGetFloatv(GL_PROJECTION_MATRIX, ProjectionMatrex);

	GLfloat tmp[16] = { 1.0,0.0,0.0,0.0,
						0.0,1.0,0.0,0.0,
						0.0,0.0,1.0,0.0,
						0.0,0.0,0.0,1.0 };

	//we manage textures by Trainvie,w class, so we modify square's render function
	square->Begin();
	//Active Texture
	glActiveTexture(GL_TEXTURE0);
	//Bind square's texture
	Textures[0]->bind();
	//pass texture to shader
	square->shaderProgram->setUniformValue("Texture", 0);

	//Call square's render function, pass ModelViewMatrex and ProjectionMatrex
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			tmp[i * 3 + j] = ModelViewMatrex[i * 3 + j];
		}
	}
	tmp[15] = ModelViewMatrex[15];
	square->Paint(ProjectionMatrex, tmp);
	square->End();



	//*********************************************************************
	// now draw the ground plane
	//*********************************************************************
	setupFloor();
	glDisable(GL_LIGHTING);
	drawFloor(200, 0);


	//*********************************************************************
	// now draw the object and we need to do it twice
	// once for real, and then once for shadows
	//*********************************************************************
	glEnable(GL_LIGHTING);
	setupObjects();
	//glDepthMask(true);
	drawStuff();

	// this time drawing is for shadows (except for top view)
	if (this->camera != 1) {
		//setupShadows();
		//drawStuff(true);
		//unsetupShadows();
	}
	terrain->Begin();
	glActiveTexture(GL_TEXTURE1);
	//Bind square's texture
	Textures[1]->bind();
	//pass texture to shader
	terrain->shaderProgram->setUniformValue("Texture1", 1);
	


	glActiveTexture(GL_TEXTURE2);
	Textures[2]->bind();

	//pass texture to shader
	terrain->shaderProgram->setUniformValue("Texture2", 2);
	

	glActiveTexture(GL_TEXTURE3);
	Textures[3]->bind();
	terrain->shaderProgram->setUniformValue("Texture3", 3);

	terrain->Paint(ProjectionMatrex, ModelViewMatrex);


	

	bump->Begin();
	glActiveTexture(GL_TEXTURE1);
	Textures[4]->bind();
	bump->shaderProgram->setUniformValue("Texture1", 1);
	glActiveTexture(GL_TEXTURE2);
	Textures[4]->bind();
	bump->shaderProgram->setUniformValue("Texture2", 2);
	glActiveTexture(GL_TEXTURE3);
	Textures[5]->bind();
	bump->shaderProgram->setUniformValue("Texture3", 3);


	bump->Paint(ProjectionMatrex, ModelViewMatrex);

	//Call triangle's render function, pass ModelViewMatrex and ProjectionMatrex
	//triangle->Paint(ProjectionMatrex, ModelViewMatrex);

	water->Paint(ProjectionMatrex, ModelViewMatrex);
	//we manage textures by Trainview class, so we modify square's render function
	
	glDepthMask(TRUE);
	GLfloat matrix[16] = {0};// = { 0 };
	GLfloat matrix1[16] = { 0 };
	GLfloat matrix2[16] = { 0 };
	GLfloat matrix3[16] = { 0 };
	GLfloat matrix4[16] = { 0 };
	GLfloat matrix5[16] = { 0 };
	GLfloat i[16] = { 0 };
	my_Identity(i);
	Pnt3f pp;
	pp.x = 0; pp.y = 20; pp.z=0;
	Pnt3f aa( 0,-20,0 );
	my_translate(matrix, pp);
	my_translate(matrix2, Pnt3f(30,0,-13));
	my_rotate(matrix1,0, -90.0 );
	my_rotate(matrix4,2, -80.0 );
	mul(matrix3, matrix2, matrix1);
	mul(matrix1, matrix3, matrix4);
	if (t_time != 0) 
	{
		rrrrrr += 1;
	}
	if (rrrrrr > 360) rrrrrr -= 360;
	my_rotate(matrix5, 2, rrrrrr);
	//mul(matrix2, matrix, matrix1);
	
	//cout << endl;
	//TTT(matrix, mat);
	GLfloat lightPos[3] = {trainAhead.x,trainAhead.y,trainAhead.z };

	//////////////////////////////
	glActiveTexture(GL_TEXTURE0);
	dummy->myTexture->bind();
	
	dummy->Paint(ProjectionMatrex, ModelViewMatrex, matrix5,lightPos,textureShader);

	glActiveTexture(GL_TEXTURE0);
	gazebo->myTexture->bind();
	gazebo->Paint(ProjectionMatrex, ModelViewMatrex, matrix1, lightPos, textureShader);


	glActiveTexture(GL_TEXTURE0);
	tree->myTexture->bind();
	tree->Paint(ProjectionMatrex, ModelViewMatrex, i, lightPos, textureShader);
	glActiveTexture(GL_TEXTURE0);
	tree1->myTexture->bind();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0);
	tree1->Paint(ProjectionMatrex, ModelViewMatrex, i, lightPos, textureShader);
	glDisable(GL_ALPHA_TEST);

	//android->Paint(ProjectionMatrex, ModelViewMatrex, matrix2, lightPos, textureShader);


	///////////////////////

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Give an empty image to OpenGL ( the last "0" )

	glDisable(GL_DEPTH_TEST);
	fbo->Begin();
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, fbo->renderedTexture);

	fbo->shaderProgram->setUniformValue("renderedTexture", 6);
	fbo->shaderProgram->setUniformValue("type", frameBuffer);
	fbo->Paint(ProjectionMatrex, ModelViewMatrex);
	fbo->End();
	glEnable(GL_DEPTH_TEST);
}

//************************************************************************
//
// * This sets up both the Projection and the ModelView matrices
//   HOWEVER: it doesn't clear the projection first (the caller handles
//   that) - its important for picking
//========================================================================
void TrainView::
setProjection()
//========================================================================
{
	// Compute the aspect ratio (we'll need it)
	float aspect = static_cast<float>(width()) / static_cast<float>(height());

	// Check whether we use the world camp
	if (this->camera == 0) {
		arcball.setProjection(false);
		update();
		// Or we use the top cam
	}
	else if (this->camera == 1) {
		float wi, he;
		if (aspect >= 1) {
			wi = 110;
			he = wi / aspect;
		}
		else {
			he = 110;
			wi = he * aspect;
		}

		// Set up the top camera drop mode to be orthogonal and set
		// up proper projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-wi, wi, -he, he, 200, -200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(-90, 1, 0, 0);
		update();
	}
	else if (this->camera == 2)
	{

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(40, aspect, 1.0, 2000.0);
		//trainFront.normalize();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		gluLookAt(trainAhead.x, trainAhead.y, trainAhead.z, trainFront.x, trainFront.y, trainFront.z, trainOrient.x, trainOrient.y, trainOrient.z);
		update();

	}
	// Or do the train view or other view here
	//####################################################################
	// TODO: 
	// put code for train view projection here!	
	//####################################################################
	else {
#ifdef EXAMPLE_SOLUTION
		trainCamView(this, aspect);
#endif
		update();
}
}

void TrainView::drawWoods(Pnt3f start, Pnt3f end, Pnt3f startOffset, Pnt3f endOffset, bool doingShadows)
{

}

float TrainView::countP(float t, int cp1, Pnt3f& qt, Pnt3f& qt1, Pnt3f& orient_t, int nowPath)
{


	spline_t type_spline = (spline_t)curve;
	/*Pnt3f cp_pos_p1 = m_pTrack->points[cp1].pos;
	Pnt3f cp_pos_p2 = m_pTrack->points[(cp1 + 1) % m_pTrack->points.size()].pos;
	Pnt3f cp_pos_p3 = m_pTrack->points[(cp1 + 2) % m_pTrack->points.size()].pos;
	Pnt3f cp_pos_p4 = m_pTrack->points[(cp1 + 3) % m_pTrack->points.size()].pos;
	//orinet
	Pnt3f cp_orient_p1 = m_pTrack->points[cp1].orient;
	Pnt3f cp_orient_p2 = m_pTrack->points[(cp1 + 1) % m_pTrack->points.size()].orient;
	Pnt3f cp_orient_p3 = m_pTrack->points[(cp1 + 2) % m_pTrack->points.size()].orient;
	Pnt3f cp_orient_p4 = m_pTrack->points[(cp1 + 3) % m_pTrack->points.size()].orient;*/
	Pnt3f cp_pos_p1 = paths[nowPath].points[cp1].pos;
	Pnt3f cp_pos_p2 = paths[nowPath].points[(cp1 + 1) % paths[nowPath].points.size()].pos;
	Pnt3f cp_pos_p3 = paths[nowPath].points[(cp1 + 2) % paths[nowPath].points.size()].pos;
	Pnt3f cp_pos_p4 = paths[nowPath].points[(cp1 + 3) % paths[nowPath].points.size()].pos;
	//orinet
	Pnt3f cp_orient_p1 = paths[nowPath].points[cp1].orient;
	Pnt3f cp_orient_p2 = paths[nowPath].points[(cp1 + 1) % paths[nowPath].points.size()].orient;
	Pnt3f cp_orient_p3 = paths[nowPath].points[(cp1 + 2) % paths[nowPath].points.size()].orient;
	Pnt3f cp_orient_p4 = paths[nowPath].points[(cp1 + 3) % paths[nowPath].points.size()].orient;
	float t1 = t + 1.0f / DIVIDE_LINE;

	mat4 G(cp_pos_p1.x, cp_pos_p2.x, cp_pos_p3.x, cp_pos_p4.x,
		cp_pos_p1.y, cp_pos_p2.y, cp_pos_p3.y, cp_pos_p4.y,
		cp_pos_p1.z, cp_pos_p2.z, cp_pos_p3.z, cp_pos_p4.z,
		0.0f, 0.0f, 0.0f, 0.0f);
	mat4 G1(cp_orient_p1.x, cp_orient_p2.x, cp_orient_p3.x, cp_orient_p4.x,
		cp_orient_p1.y, cp_orient_p2.y, cp_orient_p3.y, cp_orient_p4.y,
		cp_orient_p1.z, cp_orient_p2.z, cp_orient_p3.z, cp_orient_p4.z,
		0.0f, 0.0f, 0.0f, 0.0f);
	mat4 M0(
		-1.0f, 2.0f, -1.0f, 0.0f,
		3.0f, -5.0f, 0.0f, 2.0f,
		-3.0f, 4.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f);
	M0 *= 0.5;
	mat4 M1(
		-1.0f, 3.0f, -3.0f, 1.0f,
		3.0f, -6.0f, 0.0f, 4.0f,
		-3.0f, 3.0f, 3.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 0.0f);

	M1 /= 6.0f;
	mat4 T(t*t*t, 0.0f, 0.0f, 0.0f,
		t*t, 0.0f, 0.0f, 0.0f,
		t, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f);
	mat4 T1(t1*t1*t1, 0.0f, 0.0f, 0.0f,
		t1*t1, 0.0f, 0.0f, 0.0f,
		t1, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f);




	mat4 QT, QT1, QTO;
	switch (type_spline) {
	case spline_Linear:

		qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
		qt1 = (1 - t1) * cp_pos_p1 + t1 * cp_pos_p2;
		orient_t = (1 - t) * cp_orient_p1 + t * cp_orient_p2;
		break;

		//***********************************************************************
		//load model
	case spline_CardinalCubic:
		QT = T * M0 * G;
		QT1 = T1 * M0 * G;
		QTO = T * M0*G1;
		qt.x = QT[0][0];
		qt.y = QT[1][0];
		qt.z = QT[2][0];
		qt1.x = QT1[0][0];
		qt1.y = QT1[1][0];
		qt1.z = QT1[2][0];
		orient_t.x = QTO[0][0];
		orient_t.y = QTO[1][0];
		orient_t.z = QTO[2][0];
		break;
	case spline_CubicB_Spline:
		QT = T * M1 * G;
		QT1 = T1 * M1 * G;
		QTO = T * M0 *G1;
		qt.x = QT[0][0];
		qt.y = QT[1][0];
		qt.z = QT[2][0];
		qt1.x = QT1[0][0];
		qt1.y = QT1[1][0];
		qt1.z = QT1[2][0];
		orient_t.x = QTO[0][0];
		orient_t.y = QTO[1][0];
		orient_t.z = QTO[2][0];
		break;
	}

	return sqrt(pow(qt1.x - qt.x, 2) + pow(qt1.y - qt.y, 2) + pow(qt1.z - qt.z, 2));
}
void TrainView::setDummy(Pnt3f pos, float handRotate)
{
	glPushMatrix();
	glTranslated(pos.x, pos.y, pos.z);
	dummy_sit->render(false, false);
	glTranslated(0, 0.1, -0.95);
	if (handRotate > 0) handRotate += rand() % 30;
	if (handRotate > 100) handRotate = 100;//max angle
	glRotatef(-handRotate, 1, 0, 0);
	dummy_hand->render(false, false);
	glPopMatrix();
}
void TrainView::drawStructure(Pnt3f pos, Pnt3f orient, Pnt3f cross_t, bool doingShadows)
{
	glLineWidth(10);
	glBegin(GL_LINES);
	if (!doingShadows)
	{
		glColor3ub(0, 128, 0);
	}
	cross_t.normalize();
	cross_t = cross_t * 2.5;
	Pnt3f tmp1, tmp2, tmp3;
	tmp3 = orient;
	tmp3.normalize();
	tmp3 = tmp3 * 5;
	tmp1 = pos - cross_t - tmp3;
	tmp2 = pos + cross_t - tmp3;
	glVertex3f(tmp1.x, tmp1.y, tmp1.z);
	glVertex3f(tmp2.x, tmp2.y, tmp2.z);

	tmp1 = pos - cross_t;
	tmp2 = pos - cross_t - tmp3;
	glVertex3f(tmp1.x, tmp1.y, tmp1.z);
	glVertex3f(tmp2.x, tmp2.y, tmp2.z);

	tmp1 = pos + cross_t;
	tmp2 = pos + cross_t - tmp3;
	glVertex3f(tmp1.x, tmp1.y, tmp1.z);
	glVertex3f(tmp2.x, tmp2.y, tmp2.z);

	tmp1 = pos - tmp3;
	tmp2 = pos - tmp3;
	glVertex3f(tmp1.x, tmp1.y, tmp1.z);
	glVertex3f(tmp2.x, 0.0f, tmp2.z);
	glEnd();
}
void TrainView::my_translate(GLfloat M[16], Pnt3f pos)
{
	for (int i = 0; i < 16; i++)
	{
		M[i] = 0.0f;
	}
	M[0] = 1.0f; M[5] = 1.0f; M[10] = 1.0f; M[15] = 1.0f;
	M[3] = pos.x;
	M[7] = pos.y;
	M[11] = pos.z;
}
void TrainView::my_scale(GLfloat M[16], float s)
{
	M[0] *= s;
	M[5] *= s;
	M[10] *= s;
}
void TrainView::my_rotate(GLfloat  M[16], int axis, float angle)
{
	float arc = angle *3.14159 / 180.f;
	for (int i = 0; i < 16; i++)
	{
		M[i] = 0.0f;
	}
	if (axis == 0)
	{
		M[0] = 1.0;
		M[5] = cos(arc);
		M[6] = -sin(arc);
		M[9] = sin(arc);
		M[10] = cos(arc);
		M[15] = 1.0;
	}
	else if (axis == 1) 
	{
		
		M[0] = cos(arc);
		M[2] = sin(arc);
		M[8] = -sin(arc);
		M[10] = cos(arc);
		M[15] = 1.0;
		M[5] = 1.0;
	}
	else if (axis == 2) 
	{
		M[0] = cos(arc);
		M[1] = -sin(arc);
		M[4] = sin(arc);
		M[5] = cos(arc);
		M[15] = 1.0;
		M[10] = 1.0;
	}

}
void TrainView::my_Identity(GLfloat M[16])
{
	M[0] = 1.0f; M[5] = 1.0f; M[10] = 1.0f; M[15] = 1.0f;
}
void TrainView::mul(GLfloat  target[16], GLfloat a[16], GLfloat b[16])
{
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			for (int k = 0; k < 4; k++) 
			{
				target[i*4+j] += a[i*4+k] * b[k*4+j];
			}
			
		}
	}
	
}
/*void TrainView::TTT(GLfloat* source[4], GLfloat  target[16])
{
	int i = 0;
	for (int j = 0; j < 4; j++)
		for (int k = 0; k < 4; k++)
		{
			float temp = (float)source[j][k];
			target[i] = temp;
			i++;
		}
}*/
void TrainView::drawTrainT(Pnt3f qt, Pnt3f del, Pnt3f orient_t, float rotateD, bool doingShadow) // arc-lenght
{

	glPushMatrix();
	vec4 front = { del.x,del.y,del.z,0 };
	vec4 orient = { orient_t.x,orient_t.y,orient_t.z,0 };
	/*float Ry = atan2(del.x, del.z) / 3.1415*180.0;
	float Rx = -atan2(del.y, (sqrt(pow(del.z, 2) + pow(del.x, 2)))) / 3.1415*180.0;
	float Rz = atan2(orient_t.y, (sqrt(pow(orient_t.z, 2) + pow(orient_t.x, 2)))) / 3.1415*180.0;*/
	mat4 transf;
	glTranslatef(qt.x, qt.y, qt.z); // translate 
	//////////////////////////////////////////////////////////////////////////
	/*GLfloat m1[16] = { 0 };
	my_translate(m1, qt);*/
	

	///////////////////////////////////////////////////////////////////////////////
	//glTranslatef(qt.x+orient_t.x, qt.y+ orient_t.y, qt.z+ orient_t.z); // translate 
	float Ry = atan(front[0], front[2]);
	//////////////////////////////////////////////////////////////////////////
	/*GLfloat m2[16] = { 0 };
	my_rotate(m2, 1,Ry);
	GLfloat tmp1[16] = { 0 };
	mul(tmp1, m1, m2);*/
	///////////////////////////////////////////////////////////////////////////////
	glRotatef(Ry / 3.1415*180.0, 0, 1, 0);   // rotate around y axis
	transf = { cos(-Ry),	0.0,	sin(-Ry),	0.0,  //rotate around y axis matrix
				0.0 ,	1.0,	0.0,		0.0,
			-sin(-Ry),	0.0,	cos(-Ry),	0.0,
				0.0,	0.0,	0.0,		0.0 };
	front = front * transf;
	orient = orient * transf;
	float Rx = -atan2(front[1], front[2]);
	//////////////////////////////////////////////////////////////////////////
	/*GLfloat m3[16] = { 0 };
	my_rotate(m3, 0, Rx);
	mul(m1, tmp1, m3);*/

	///////////////////////////////////////////////////////////////////////////////
	glRotatef(Rx / 3.1415*180.0, 1, 0, 0);// rotate around x axis
	transf = { 1.0,	0.0,		0.0,		0.0,	//rotate around x axis matrix
				0.0,	cos(-Rx),	-sin(-Rx),	0.0,
				0.0,	sin(-Rx),	cos(-Rx),	0.0,
				0.0,	0.0,		0.0,		0.0 };
	//front = front * transf; //useless
	orient = orient * transf;
	float Rz = atan2(orient[1], orient[0]);
	//////////////////////////////////////////////////////////////////////////
	/*GLfloat m4[16] = { 0 };
	my_rotate(m4, 2, (Rz-(90*3.1415/180.0)));
	mul(tmp1, m1, m4);
*/
	///////////////////////////////////////////////////////////////////////////////
	glRotatef((Rz / 3.1415*180.0) - 90, 0, 0, 1); // rotate around z axis
	glTranslated(0.0, 3.0, 0.0);
	//////////////////////////////////////////////////////////////////////////
	/*GLfloat m5[16] = { 0 };
	my_translate(m5,Pnt3f(0.0, 1.0, 0.0));
	mul(m1, tmp1, m5);*/

	///////////////////////////////////////////////////////////////////////////////
	//RCcar->Paint(ProjectionMatrex, ModelViewMatrex, m1, 0);
	if (particleC != 3)
	{

		m->render(false, false);
		float handRotate = 0;
		del.normalize();
		if (del.y < -0.01)
		{
			handRotate = -(120 * del.y);
			handRotate -= 10;
		}

		if (!doingShadow)	glColor3f(1, 0, 0);	setDummy(Pnt3f(1.5f, 1.0f, -1.5f), handRotate);
		if (!doingShadow)	glColor3f(1, 1, 0);	setDummy(Pnt3f(-1.5f, 1.0f, -1.5f), handRotate);
		if (!doingShadow)	glColor3f(0, 1, 0);	setDummy(Pnt3f(1.5f, 1.0f, 2.0f), handRotate);
		if (!doingShadow)	glColor3f(0, 0, 1);	setDummy(Pnt3f(-1.5f, 1.0f, 2.0f), handRotate);
		float circumference = 2.0*r*3.14159;
		if (rotateD > circumference)
		{
			rotateD -= circumference;
		}
		else if (rotateD < 0.0f)
		{
			rotateD += circumference;
		}
		if (!doingShadow)
		{
			glColor3ub(0x30, 0x0, 0xff);

		}
		float percentA = rotateD / circumference;
		glPushMatrix();
		glPushMatrix();
		glTranslated(3.0, -1.0, 3);
		glRotatef(percentA*360.0, 1, 0, 0);

		leftWheel->render();
		glPopMatrix();
		glPushMatrix();
		glTranslated(3.0, -1.0, -3);
		glRotatef(percentA*360.0, 1, 0, 0);

		leftWheel->render();
		glPopMatrix();

		glPopMatrix();

		glPushMatrix();
		glPushMatrix();
		glTranslated(-3.0, -1.0, 3);
		glRotatef(percentA*360.0, 1, 0, 0);
		rightWheel->render();
		glPopMatrix();
		glPushMatrix();
		glTranslated(-3.0, -1.0, -3);
		glRotatef(percentA*360.0, 1, 0, 0);
		rightWheel->render();
		glPopMatrix();
		glPopMatrix();
	}
	else 
	{
		mineCraftCar->render(false, false);
	}
		glPopMatrix();
	
}

//************************************************************************
//
// * this draws all of the stuff in the world
//
//	NOTE: if you're drawing shadows, DO NOT set colors (otherwise, you get 
//       colored shadows). this gets called twice per draw 
//       -- once for the objects, once for the shadows
//########################################################################
// TODO: 
// if you have other objects in the world, make sure to draw them
//########################################################################
//========================================================================
void TrainView::drawStuff(bool doingShadows)
{
	spline_t type_spline = (spline_t)curve;
	// Draw the control points
	// don't draw the control points if you're driving 
	// (otherwise you get sea-sick as you drive through them)
	if (this->camera != 2) {
		for (size_t i = 0; i < this->m_pTrack->points.size(); ++i) {
			if (!doingShadows) {
				if (((int)i) != selectedCube)
					glColor3ub(240, 60, 60);
				else
					glColor3ub(240, 240, 30);
			}
			this->m_pTrack->points[i].draw();
		}
		update();
	}

	// draw the track
	//####################################################################
	// TODO: 
	// call your own track drawing code

	float acceleration = 0.0f;
	float woodDisCnt = 0.0f;
	float disCount = 0.0f;
	bool isDrew = false;
	Pnt3f drawTrainBuf;
	Pnt3f drawTrainPosBuf; //vector3
	Pnt3f drawTrainOrientBuf;
	Pnt3f woodStart, woodEnd, woodStartCross, woodEndCross;
	bool isStart = false;
	bool isChanged = false;
	if (trackBuffer.points.size() != m_pTrack->points.size() || splineBuffer != type_spline)
	{
		isChanged = true;
	}
	else
	{
		for (size_t i = 0; i < m_pTrack->points.size(); i++)
		{
			if (m_pTrack->points[i].pos.x != trackBuffer.points[i].pos.x || m_pTrack->points[i].pos.y != trackBuffer.points[i].pos.y || m_pTrack->points[i].pos.z != trackBuffer.points[i].pos.z)
			{
				isChanged = true;
				break;
			}
			if (m_pTrack->points[i].orient.x != trackBuffer.points[i].orient.x || m_pTrack->points[i].orient.y != trackBuffer.points[i].orient.y || m_pTrack->points[i].orient.z != trackBuffer.points[i].orient.z)
			{
				isChanged = true;
			}
		}
	}
	if (isChanged)
	{
		p2pDistance.clear();
		pathP2PD.clear();
		paths.clear();
		splineBuffer = type_spline;
		trackBuffer.points.clear();
		for (size_t i = 0; i < m_pTrack->points.size(); i++)
		{
			trackBuffer.points.push_back(m_pTrack->points[i]);

		}

		for (int i = 0; i < 2; i++)
		{
			CTrack tmp;
			tmp.points.clear();
			for (int j = 0; j < m_pTrack->points.size(); j++)
			{
				if (i == 0)
				{
					if (j != 1)
					{
						tmp.points.push_back(m_pTrack->points[j]);
					}
				}
				else if (i == 1)
				{
					tmp.points.push_back(m_pTrack->points[j]);
				}
			}
			paths.push_back(tmp);
		}

	}
	for (int j = 0; j < paths.size(); j++)
	{
		if (isChanged)
			pathP2PD.push_back(vector<float>(0.0));
		//for (size_t i = 0; i < m_pTrack->points.size(); i++)
		for (size_t i = 0; i < paths[j].points.size(); i++)
		{



			/*//pos
			Pnt3f cp_pos_p1 = m_pTrack->points[i].pos;
			Pnt3f cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
			Pnt3f cp_pos_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].pos;
			Pnt3f cp_pos_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].pos;
			//orinet
			Pnt3f cp_orient_p1 = m_pTrack->points[i].orient;
			Pnt3f cp_orient_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].orient;
			Pnt3f cp_orient_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].orient;
			Pnt3f cp_orient_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].orient;
			*/

			Pnt3f cp_pos_p1 = paths[j].points[i].pos;
			Pnt3f cp_pos_p2 = paths[j].points[(i + 1) % paths[j].points.size()].pos;
			Pnt3f cp_pos_p3 = paths[j].points[(i + 2) % paths[j].points.size()].pos;
			Pnt3f cp_pos_p4 = paths[j].points[(i + 3) % paths[j].points.size()].pos;
			//orinet
			Pnt3f cp_orient_p1 = paths[j].points[i].orient;
			Pnt3f cp_orient_p2 = paths[j].points[(i + 1) % paths[j].points.size()].orient;
			Pnt3f cp_orient_p3 = paths[j].points[(i + 2) % paths[j].points.size()].orient;
			Pnt3f cp_orient_p4 = paths[j].points[(i + 3) % paths[j].points.size()].orient;



			float percent = 1.0f / DIVIDE_LINE;
			float t = 0.0f;
			//Pnt3f qt = (1.0f - t) * cp_pos_p1 + t * cp_pos_p2;
			Pnt3f qt0, qt1;
			Pnt3f orient_t;
			GLfloat G[16] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };



			if (isChanged)
			{
				/*p2pDistance.push_back(0.0);
				for (size_t j = 0; j < DIVIDE_LINE; j++)// 衡琿禯瞒
				{


					float distance = countP((float)j / DIVIDE_LINE, i, qt0, qt1, orient_t);

					p2pDistance[i] += distance;

				}*/

				pathP2PD[j].push_back(0.0);
				for (int k = 0; k < DIVIDE_LINE; k++)
				{

					float distance = countP((float)k / DIVIDE_LINE, i, qt0, qt1, orient_t, j);
					pathP2PD[j][i] += distance;

				}


			}
			//=================draw line====================
			lineTest->Paint(ProjectionMatrex, ModelViewMatrex, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, type_spline, 0); // draw by shader
			lineTest->Paint(ProjectionMatrex, ModelViewMatrex, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, type_spline, 1); // draw by shader
			// ================= draw sleeper===================
			float space = 6.0f;
			float sleeperWidth = 1.0f;
			/*for (int j = 0; j < ((p2pDistance[i] - offset)) / space; j++)
			{
				float ratio = (space * (float)j + offset) / p2pDistance[i];
				float ratio2 = ratio + (sleeperWidth / (p2pDistance[i]));
				if (ratio2 > 1.0)
				{
					//offset = (ratio2 - 1.0f)*(p2pDistance[i]);
					ratio2 = 1;
				}
				else if (j == (((p2pDistance[i]) + offset) / space) - 1)
				{
					//offset = (float)j*space+sleeperWidth- p2pDistance[i];
				}
				sleeper->Paint(ProjectionMatrex, ModelViewMatrex, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, type_spline, ratio, ratio2); // draw by shader
			}*/
			for (int k = 0; k < ((pathP2PD[j][i] - offset)) / space; k++)
			{
				float ratio = (space * (float)k + offset) / pathP2PD[j][i];
				float ratio2 = ratio + (sleeperWidth / (pathP2PD[j][i]));
				if (ratio2 > 1.0)
				{
					ratio2 = 1;
				}
				else if (k == (((pathP2PD[j][i]) + offset) / space) - 1)
				{
					//offset = (float)j*space+sleeperWidth- p2pDistance[i];
				}
				sleeper->Paint(ProjectionMatrex, ModelViewMatrex, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, type_spline, ratio, ratio2); // draw by shader
			}
			//==============tunnel================
			if (paths[j].points.size() > 5)
			{
				if (j == 1 && (i == 3 || i == 4))
				{
					for (int k = 0; k < DIVIDE_LINE; k++)
					{
						float ratio1 = (float)k / DIVIDE_LINE;
						float ratio2 = (float)(k + 1.0) / DIVIDE_LINE;


						if ((ratio1 > 0.6&&i == 3) || (i == 4 && ratio1 < 0.8))
							tunnel->Paint(ProjectionMatrex, ModelViewMatrex, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, type_spline, ratio1, ratio2); // draw by shader
					}
				}
			}
			//structure

			for (float k = 0; k < 1; k += 0.25)
			{
				Pnt3f pos, orient, cross, pos1, del;
				countP(k, i, pos, pos1, orient, j);
				del = pos1 - pos;
				del.normalize();
				orient.normalize();
				cross = del * orient;
				drawStructure(pos, orient, cross, doingShadows);
			}
		}
	}
	isChanged = false;
	//==================train position====================
	/*if (t_time != 0.0)
	{
		trainMoveDistance += speed * t_time;
		rotateD += speed * t_time;
		if (trainMoveDistance > p2pDistance[trainCP])
		{
			trainMoveDistance -= p2pDistance[trainCP];
			trainCP = (trainCP + 1) % m_pTrack->points.size();
		}
		else if (trainMoveDistance < 0)
		{
			trainCP = (trainCP + m_pTrack->points.size() - 1) % m_pTrack->points.size();
			trainMoveDistance += p2pDistance[trainCP];
		}

	}
	float carLenght = 15.0f;
	float tmpTMD = trainMoveDistance;
	int tmpTCP = trainCP;
	for (int j = 0; j < 5; j++)
	{
		float percent = tmpTMD / p2pDistance[tmpTCP];
		Pnt3f qt1, qt0, orient_t;
		countP(percent, tmpTCP, qt0, qt1, orient_t);//衡 qt0 delta orient_t
		Pnt3f delta = qt1 - qt0;
		delta.normalize();
		//speed = 50 - delta.y*40.0;
		if (j == 0)
		{

			acceleration = 0.001f;
			speed += t_time * 60 * acceleration;
			speed -= (delta.y*25.0)*t_time ;
			t_time = 0.0;
		}
		if (!doingShadows)
		{
			glColor3ub(0x99, 0x93, 0x99);
		}
		drawTrainT(qt0, delta, orient_t, rotateD, doingShadows);
		tmpTMD -= carLenght;

		if (tmpTMD > p2pDistance[tmpTCP])
		{
			tmpTMD -= p2pDistance[tmpTCP];
			tmpTCP = (tmpTCP + 1) % m_pTrack->points.size();
		}
		else if (tmpTMD < 0)
		{
			tmpTCP = (tmpTCP + m_pTrack->points.size() - 1) % m_pTrack->points.size();
			tmpTMD += p2pDistance[tmpTCP];
		}

	}*/
	//だ瓂 array!@#$#%#

	if (t_time != 0.0)
	{
		trainMoveDistance += speed * t_time;
		rotateD += speed * t_time;
		if (trainCP == ((paths[nowPath].points.size()-4)% paths[nowPath].points.size()))
		{
			if (pathControl == 1)
			
			{
				nowPath = 1;
				trainCP = (paths[nowPath].points.size() - 4) % paths[nowPath].points.size();
			}
				
			else if (pathControl == 0) 
			{
				nowPath = 0;
				trainCP = (paths[nowPath].points.size() - 4) % paths[nowPath].points.size();
			}
		}
		if (trainMoveDistance > pathP2PD[nowPath][trainCP])
		{
			trainMoveDistance -= pathP2PD[nowPath][trainCP];
			trainCP = (trainCP + 1) % paths[nowPath].points.size();
		}
		else if (trainMoveDistance < 0)
		{
			trainCP = (trainCP + paths[nowPath].points.size() - 1) % paths[nowPath].points.size();
			trainMoveDistance += pathP2PD[nowPath][trainCP];
		}

	}
	float carLenght = 13.0f;
	if (particleC != 3) 
	{
		carLenght = 13.0f;
	}else
	{
		carLenght = 15.0f;
	}
	float tmpTMD = trainMoveDistance;
	int tmpTCP = trainCP;
	for (int j = 0; j < 5; j++)
	{
		float Cpercent = (float)j / 5.0;
		float colorR = 10 * Cpercent + (1.0 - Cpercent) * 200;
		float colorG = 220 * Cpercent + (1.0 - Cpercent) * 30;
		float colorB=  40 * Cpercent + (1.0 - Cpercent) * 150;
		
		float percent = tmpTMD / pathP2PD[nowPath][tmpTCP];
		Pnt3f qt1, qt0, orient_t;
		countP(percent, tmpTCP, qt0, qt1, orient_t, nowPath);//衡 qt0 delta orient_t
		Pnt3f delta = qt1 - qt0;
		orient_t.normalize();
		delta.normalize();
		//speed = 50 - delta.y*40.0;
		if (j == 0)
		{
			trainAhead = qt0 + orient_t * 6 + delta * 3;
			trainFront = qt1 + orient_t * 6 + delta * 3;
			trainOrient = orient_t;
			acceleration = 0.001f;
			speed += t_time * 60 * acceleration;
			speed -= (delta.y*40.0)*t_time;
			t_time = 0.0;
		}
		if (j == 4) 
		{
			lastCarPos = qt0;
		}
		if (j == 0)
		{
			if (particleC == 0)
			{
				ProcessParticles(lastCarPos.x, lastCarPos.y, lastCarPos.z);
				DrawParticles();
			}
		}
		if(!doingShadows)
			glColor3ub(colorR, colorG, colorB);
		
		drawTrainT(qt0, delta, orient_t, rotateD, doingShadows);
		tmpTMD -= carLenght;

		if (tmpTMD > pathP2PD[nowPath][tmpTCP])
		{
			tmpTMD -= pathP2PD[nowPath][tmpTCP];
			tmpTCP = (tmpTCP + 1) % paths[nowPath].points.size();
		}
		else if (tmpTMD < 0)
		{
			tmpTCP = (tmpTCP + paths[nowPath].points.size() - 1) % paths[nowPath].points.size();
			tmpTMD += pathP2PD[nowPath][tmpTCP];
		}

	}

	








	//####################################################################

#ifdef EXAMPLE_SOLUTION
	drawTrack(this, doingShadows);
#endif

	// draw the train
	//####################################################################
	// TODO: 
	//	call your own train drawing code
	//####################################################################

	//drawTrainT(drawTrainPosBuf,drawTrainBuf, drawTrainOrientBuf,doingShadows);



#ifdef EXAMPLE_SOLUTION
	// don't draw the train if you're looking out the front window
	if (!tw->trainCam->value())
		drawTrain(this, doingShadows);
#endif
}

void TrainView::
doPick(int mx, int my)
//========================================================================
{
	// since we'll need to do some GL stuff so we make this window as 
	// active window
	makeCurrent();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPickMatrix((double)mx, (double)(viewport[3] - my),
		5, 5, viewport);

	// now set up the projection
	setProjection();

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100, buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);


	// draw the cubes, loading the names as we go
	for (size_t i = 0; i < m_pTrack->points.size(); ++i) {
		glLoadName((GLuint)(i + 1));
		m_pTrack->points[i].draw();
	}

	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selectedCube = buf[3] - 1;
	}
	else // nothing hit, nothing selected
		selectedCube = -1;
}
extern pParticle Particles;
extern UINT nOfFires;
extern UINT Tick1, Tick2;
extern float DTick;
extern GLuint textureID;
void ProcessParticles(float x, float y, float z)
{
	Tick1 = Tick2;
	Tick2 = GetTickCount();
	DTick = float(Tick2 - Tick1);
	DTick *= 0.5f;

	Particle ep;
	if (nOfFires < MAX_FIRES)
	{
		InitParticle_smoke(ep);
		InitParticle_fountain(ep);
		InitParticle_star(ep);
		InitParticle_fire(ep, x, y, z);
	}
	pParticle par;
	par = Particles;

	while (par)
	{
		par->life -= par->fade * (float(DTick) * 0.1f);//Particle关㏑癐搭?
		if (par->life <= 0.05f)
		{//讽关㏑﹚??????????
			if (par->nExpl)
			{//脄狦
				switch (par->nExpl)
				{
				case 1:
					Explosion1(par);
					break;
				case 2:
					Explosion2(par);
					break;
				case 3:
					Explosion3(par);
					break;
				case 4:
					Explosion4(par);
					break;
				case 5:
					Explosion5(par);
					break;
				case 6:
					Explosion6(par);
					break;
				case 7:
					Explosion7(par);
					break;
				case 8:
					Explosion8(par);
					break;
				case 9:
					Explosion9(par);
					break;
				case 10:
					Explosion10(par, x, y, z);
					break;
				case 11:
					Explosion11(par);
					break;
				default:
					break;
				}
			}
			if (par->bFire)
				nOfFires--;
			DeleteParticle(&par);
		}
		else {
			par->xpos += par->xspeed*DTick;
			par->ypos += par->yspeed*DTick;
			par->zpos += par->zspeed*DTick;
			par->yspeed -= par->gravity * DTick;
			if (par->nExpl == 11)
			{
				par->r = sin(2 * (par->life / 0.8f)*3.1415926);
				par->g = sin(2 * (par->life / 0.8f)*3.1415926);
				par->b = sin(2 * (par->life / 0.8f)*3.1415926);
			}
			if (par->bAddParts)
			{//安ΤЮぺ??
				par->AddCount += 0.01f*DTick;//AddCount跑て稶篊Юぺ采稶??
				if (par->AddCount > par->AddSpeed) {//AddSpeed稶Юぺ采稶??
					par->AddCount = 0;
					ep.b = par->b;
					ep.g = par->g;
					ep.r = par->r;
					ep.life = par->life*0.9f;//关㏑跑祏??
					ep.fade = par->fade*7.0f;//癐?еㄇ??
					ep.size = par->size*0.7f;//采へㄇ??
					ep.xpos = par->xpos;
					ep.ypos = par->ypos;
					ep.zpos = par->zpos;
					ep.xspeed = 0.0f;
					ep.yspeed = 0.0f;
					ep.zspeed = 0.0f;
					ep.bFire = 0;
					ep.nExpl = 0;
					ep.bAddParts = 0;//Юぺ采?ΤЮぺ??
					ep.AddCount = 0.0f;
					ep.AddSpeed = 0.0f;
					AddParticle(ep);
				}
			}
			par = par->pNext;//穝采
		}
	}
}