#include "shape.h"

void Shape::set_curve_point()
{
    QPoint start = my_curve[0],end = my_curve[3];
    int x = start.x()*2/3.0+end.x()/3.0;
    int y = start.y()*2/3.0+end.y()/3.0;
    my_curve[1]=QPoint(x,y);
    x = start.x()/3.0+end.x()*2/3.0;
    y = start.y()/3.0+end.y()*2/3.0;
    my_curve[2]=QPoint(x,y);
}

void Shape::set_shape_point()
{
    int max_x=0,min_x=INT_MAX,max_y=0,min_y = INT_MAX;
    QPoint a = startpoint,b = endpoint;
    max_x = (a.x() > b.x())?a.x():b.x();
    max_y = (a.y() > b.y())?a.y():b.y();
    min_x = (a.x() < b.x())?a.x():b.x();
    min_y = (a.y() < b.y())?a.y():b.y();
    QPoint c(min_x,min_y);
    QPoint d(max_x,min_y);
    QPoint e(max_x,max_y);
    QPoint f(min_x,max_y);
    my_curve[0] = c;
    my_curve[1] = d;
    my_curve[2] = e;
    my_curve[3] = f;
}
