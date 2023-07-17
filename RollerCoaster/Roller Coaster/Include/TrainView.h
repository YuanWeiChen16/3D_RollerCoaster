#ifndef TRAINVIEW_H  
#define TRAINVIEW_H  
#include <QGLWidget> 
#include <QtGui>  
#include <QtOpenGL>  
#include <GL/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib") 
#include "Utilities/ArcBallCam.H"
#include "Utilities/3DUtils.H"
#include "Track.H"
#include "Triangle.h"
#include "Square.h"
#include <glm/glm/glm.hpp>
#include <Point3d.h>
#include <Model.h>
#include <vector.h>
#include <3DSLoader.h>
#include <CBMPLoader.h>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QtMultimedia/QSound>
#include <cmath>
#include "TrackTest.h";
#include "Sleeper.h"
#include "Tunnel.h"
#include "partical.h"
#include "Terrain.h"
#include "myModel.h"
#include "Bump.h"
#include "FBO.h"
#define DIVIDE_LINE 500.0
using namespace glm;
class AppMain;
class CTrack;

//#######################################################################
// TODO
// You might change the TrainView in order to add different objects to
// be drawn, or adjust the lighting, or ...
//#######################################################################
typedef enum {
	spline_Linear = 0,
	spline_CardinalCubic = 1,
	spline_CubicB_Spline = 2
} spline_t;





class TrainView : public QGLWidget, protected QOpenGLFunctions_4_3_Core
{  
	Q_OBJECT  
public:  
	explicit TrainView(QWidget *parent = 0);  
	~TrainView();  

public:
	// overrides of important window things
	//virtual int handle(int);
	virtual void paintGL();

	// all of the actual drawing happens in this routine
	// it has to be encapsulated, since we draw differently if
	// we're drawing shadows (no colors, for example)
	void drawStuff(bool doingShadows=false);

	// setup the projection - assuming that the projection stack has been
	// cleared for you
	void setProjection();
	void drawWoods(Pnt3f start, Pnt3f end, Pnt3f startOffset,Pnt3f endOffset, bool doingShadow);
	float countP(float t, int cp1, Pnt3f& qt, Pnt3f& qt1, Pnt3f& orient_t,int nowPath);
	void drawTrainT(Pnt3f qt,Pnt3f del, Pnt3f orient_t,float rotateD,bool doingShadow);
	// Reset the Arc ball control
	void resetArcball();

	// pick a point (for when the mouse goes down)
	void doPick(int mx, int my);

	void initializeGL();
	void initializeTexture();


public:
	ArcBallCam		arcball;			// keep an ArcBall for the UI
	int				selectedCube;  // simple - just remember which cube is selected

	CTrack*			m_pTrack;		// The track of the entire scene

	QSound *sound = new QSound("fire1.wav");
	int camera;
	int curve;
	int track;
	bool isrun;
	float t_time = 0;
	Triangle* triangle;
	Square* square;
	GLfloat ProjectionMatrex[16];
	GLfloat ModelViewMatrex[16];
	QVector<QOpenGLTexture*> Textures;

	Water* water;
	//========== draw track by shader ===========
	TrackTest* lineTest;
	CTrack			trackBuffer;	//¬ö¿ýcontrol point;
	int splineBuffer;
	vector<float> p2pDistance; // controlpoint to controlpoint distance
	float trainMoveDistance = 0; // train move distance between control point and control point
	int trainCP = 0;
	vector<CTrack> paths; 
	vector<vector<float>> pathP2PD;
	int pathControl = 0;
	int nowPath = 0;
	

	//train
	Model* m;
	Model* leftWheel;
	Model* rightWheel;
	Model* dummy_sit;
	Model* dummy_hand;
	float speed = 50;
	void setDummy(Pnt3f pos, float handRotate);
	Pnt3f trainAhead,trainFront,trainOrient;
	Model* mineCraftCar;
	

	//sleeper
	Sleeper* sleeper;
	float offset = 0;

	//tunnel
	Tunnel* tunnel;

	//wheel
	float rotateD =0.0;
	float r = 3.0;

	//particle
	Pnt3f lastCarPos, lastCarO, lastCarC;


	//terrain
	Terrain* terrain;
	Bump* bump;

	//structure
	void drawStructure(Pnt3f pos, Pnt3f orient, Pnt3f cross,bool doingShadows);


	//model
	MyModel *dummy;
	MyModel *RCcar;
	MyModel *gazebo;

	MyModel *tree;
	MyModel *tree1;
	//MyModel *android;
	//
	int particleC = 0 ;
	int textureShader = 0;
	int frameBuffer = 0;
	/////
	void my_translate(GLfloat M[16], Pnt3f pos);
	void my_scale(GLfloat M[16], float s);
	void my_rotate(GLfloat M[16], int axis,float angle);
	void my_Identity(GLfloat M[16]);
	void mul(GLfloat target[16], GLfloat a[16], GLfloat b[16]);
	//void TTT(GLfloat source, GLfloat  target[16]);
	//
	FBO* fbo;

	float rrrrrr = 0;
};  
#endif // TRAINVIEW_H  
void ProcessParticles(float x, float y, float z);