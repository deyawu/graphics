#include "openglw.h"

openglw::openglw(QString file,QWidget *parent):QOpenGLWidget (parent)
{
    filename = file;
    f_angle = 0;
}


void openglw::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    read_from_file(filename);
}
void openglw::resizeGL(int w,int h)
{

}
void openglw::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(f_angle,1.0f,1.0f,1.0f);


    for(int i = 0;i < faces;i++)
    {
        glBegin(GL_TRIANGLES);
        glColor3f(0.0f, 0.0f, 1.0f);
        for(int j = 0;j < num;j++)
        {
            switch(j%num)
            {
            case 0: glColor3f(1.0f, 0.0f, 0.0f); ;break;
            case 1: glColor3f(0.0f, 1.0f, 0.0f); break;
            case 2: glColor3f(0.0f, 0.0f, 1.0f); break;
            }
            Vertex *p = points[face_points[i][j]];
            glVertex3f(p->x,p->y,p->z);
        }
        glEnd();
    }
    f_angle  = (f_angle + 1)%360;
    glFlush();
}

void openglw::read_from_file(QString &fname)
{
    string filename = fname.toStdString();
    ifstream infile(filename,ios::in);
    if(!infile)
    {
        cout<<"wrong"<<endl;
    }
    else
    {
        string a,b,c;
        infile >>a ;
        if(a != "OFF")
        {
            cout<<"not off file!"<<endl;
            return;
        }
        else
        {
            infile>>a>>b>>c;
            vertexs = stoi(a);
            faces = stoi(b);
            for(int i = 0;i < vertexs;i++)
            {
                infile>>a>>b>>c;
                Vertex *m = new Vertex(stof(a),stof(b),stof(c));
                points.push_back(m);
            }
            for(int i = 0;i < faces;i++)
            {
                vector<int>temp;
                infile >>a;
                 num = stoi(a);
                for(int j = 0;j < num;j++)
                {
                    infile >>a;
                    temp.push_back(stoi(a));
                }
                face_points.push_back(temp);
            }
        }
    }
}
