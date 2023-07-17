#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLShaderProgram>
#include <QVector>
#include <QVector3D>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include "Track.H"

class TrackTest
{
public:
	QOpenGLShaderProgram* shaderProgram;
	QOpenGLShader* vertexShader;
	QOpenGLShader* fragmentShader;
	QVector<QVector3D> vertices;
	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vvbo;
	CTrack* track_p;
	float divide_line ;

public:
	TrackTest();
	void Init(int divide, CTrack* track);
	void InitVAO();
	void InitVBO();
	void InitShader(QString vertexShaderPath, QString fragmentShaderPath);
	void Paint(GLfloat* ProjectionMatrix, GLfloat* ModelViewMatrix, Pnt3f controlP1, Pnt3f controlP2, Pnt3f controlP3, Pnt3f controlP4,Pnt3f orientP1,Pnt3f orientP2,Pnt3f orientP3,Pnt3f orientP4,int curveType,int in_or_out);
	void DimensionTransformation(GLfloat source[], GLfloat target[][4]);

};