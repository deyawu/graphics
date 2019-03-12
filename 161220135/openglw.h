#ifndef OPENGLW_H
#define OPENGLW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>
#include <vector>

#include <iostream>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <fstream>
#include <windows.h>
#include <qopenglextrafunctions.h>
#include <QGLWidget>
using namespace  std;

struct Vertex
{
    float x,y,z;
    Vertex(float a,float b,float c){x=a;y=b;z=c;}
};


class openglw:public QOpenGLWidget,protected QOpenGLFunctions_3_0
{
    Q_OBJECT
public:
    openglw(QString file,QWidget *parent = 0);
    ~openglw(){}
    void read_from_file(QString &fname);
    void initializeGL();
    void resizeGL(int w,int h);
    void paintGL();

public slots:
    void update_3d(){update();}


private:
    vector<Vertex*>points;
    int vertexs,faces,num;
    vector<vector<int>> face_points;
    QString filename;
    int f_angle;
};

#endif // OPENGLW_H
