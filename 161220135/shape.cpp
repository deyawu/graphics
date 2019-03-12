#include "shape.h"

Shape::Shape()
{

    this->shape_color = QColor(Qt::black);
    this->startpoint=QPoint(0,0);
    this->endpoint=QPoint(0,0);
}

Shape::~Shape()
{

}

void Shape::my_draw_point(int x,int y,QPainter *my_painter)
{
    QPoint pointf;
    pointf.setX((int)x);
    pointf.setY((int)y);
    my_painter->drawPoint(pointf);
}
void Shape::drawing_line(QPoint start,QPoint end,QPainter *my_painter)
{
    int startx = start.x(),starty = start.y(),endx=end.x(),endy=end.y();
    int dietx =endx-startx,diety = endy-starty;
    int maxsteps = abs(dietx) > abs(diety)?abs(dietx):abs(diety);
    double x=startx,y=starty;
    double xi=((double)dietx)/maxsteps,yi=((double)diety)/maxsteps;
    for(int label = 1;label<=maxsteps;label++)
    {
        x += xi;
        y +=yi;
        my_painter->drawPoint(QPoint(x,y));
    }
}
void Shape::my_draw_pen(QPainter *my_painter)
{
    for(int i = 0;i < my_pen.size();i++)
    {
        my_draw_point(my_pen[i].x(),my_pen[i].y(),my_painter);
    }
}
void Shape::my_draw_curve(QPainter *my_painter)
{
    if(my_curve.size() == 4)
    {
        drawing_line(my_curve[0],my_curve[3],my_painter);
    }
    else
    {
        double n = 1000;
        int draw_x = 0,draw_y = 0;
        float start_x = my_curve[0].x(),end_x=my_curve[3].x(),mid_1_x = my_curve[1].x(),mid_2_x=my_curve[2].x();
        float start_y = my_curve[0].y(),end_y=my_curve[3].y(),mid_1_y = my_curve[1].y(),mid_2_y=my_curve[2].y();
        for(int i = 1;i < n;i++)             // 公式是否使用正确
        {
            double u = i/n;
            draw_x = start_x*(pow(1-u,3)) + 3*mid_1_x*u*pow(1-u,2) + 3*mid_2_x*pow(u,2)*(1-u) + end_x*pow(u,3);
            draw_y = start_y*(pow(1-u,3)) + 3*mid_1_y*u*pow(1-u,2) + 3*mid_2_y*pow(u,2)*(1-u) + end_y*pow(u,3);
            my_draw_point(draw_x,draw_y,my_painter);
        }
    }
}
void Shape::my_draw_line(QPainter *my_painter)
{
    drawing_line(startpoint,endpoint,my_painter);
}

void  Shape::my_draw_ellipse(QPainter *my_painter)
{
    QPoint start = my_curve[0],end = my_curve[2];
    int center_x = (start.x()+end.x())/2, center_y = (start.y()+end.y())/2;  // center position of ellipse
    double rx = abs((end.x()-start.x())/2),ry = abs((end.y()-start.y())/2); // len of a and b
    double x = 0,y = ry;
    double a1_p1 = ry*ry+rx*rx/4-rx*rx*ry;
    my_draw_point(center_x,center_y+y,my_painter);
    my_draw_point(center_x,center_y-y,my_painter);
    while(ry*ry*x < rx*rx*y)
    {
        if(a1_p1 < 0)
        {
            x = x+1;
            a1_p1 += ry*ry + 2*ry*ry*x;
        }
        else
        {
            x = x+1;
            y=y-1;
            a1_p1 += 2*ry*ry*x-2*rx*rx*y+ry*ry;
        }
        my_draw_point(center_x+x,center_y+y,my_painter);
        my_draw_point(center_x+x,center_y-y,my_painter);
        my_draw_point(center_x-x,center_y+y,my_painter);
        my_draw_point(center_x-x,center_y-y,my_painter);

    }
    double a2_p2 = sqrt(ry*(x+0.5)) + sqrt(rx)*(y-1) - sqrt(rx*ry);
    while(y>=0)
    {
        if(a2_p2 > 0)
        {
            y=y-1;
            a2_p2=a2_p2-2*rx*rx*y+rx*rx;
        }
        else
        {
            x = x+1;
            y=y-1;
            a2_p2 = a2_p2 + 2*ry*ry*x - 2*rx*rx*y+rx*rx;
        }
        my_draw_point(center_x+x,center_y+y,my_painter);
        my_draw_point(center_x+x,center_y-y,my_painter);
        my_draw_point(center_x-x,center_y+y,my_painter);
        my_draw_point(center_x-x,center_y-y,my_painter);
    }
}

void Shape::my_draw_dot_line(QPainter *my_painter)
{
    QPen pen;
    QColor p(0,0,0);
    pen.setWidth(2);
    pen.setStyle(Qt::DashLine);
    pen.setColor(p);
    my_painter->setPen(pen);
    int w = abs(my_curve[0].x()-my_curve[2].x()),h = abs(my_curve[0].y()-my_curve[2].y());
    my_painter->drawRect(my_curve[0].x(),my_curve[0].y(),w,h);
}

void  Shape::my_draw_rect(QPainter *my_painter)
{
    int i = 0;
    for(;i < my_curve.size()-1;i++)
    {
        drawing_line(my_curve[i],my_curve[i+1],my_painter);
    }
    drawing_line(my_curve[i],my_curve[0],my_painter);
}

void  Shape::my_draw_polygon(QPainter *my_painter)
{
    if(my_curve.size() == 1)
    {
        my_painter->drawEllipse(my_curve[0].x()-1,my_curve[0].y()-1,2,2);
    }
    else if(my_curve.size() > 1)
    {
        for(int i  = 0;i < my_curve.size()-1;i++)
        {
            drawing_line(my_curve[i],my_curve[i+1],my_painter);
        }
    }
}
