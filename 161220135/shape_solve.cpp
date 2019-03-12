#include "shape_solve.h"

Shape_solve::Shape_solve()
{
    s_max_x=0;
    s_min_x=INT_MAX;
    s_max_y=0;
    s_min_y = INT_MAX;
    dotsize = 8;
}
void Shape_solve::output_message(string c)
{

}


bool Shape_solve::Inside_Last_Shape(QPoint f,QPoint start,QPoint end,int size)
{
    return ((f.x()>s_min_x-2*size)&&(f.x()<s_max_x-2*size)&&(f.y()>s_min_y+2*size)&&(f.y()<s_max_y-2*size));
}

bool Shape_solve::On_Dot_Shape(QPoint f,int size)
{
 /*   bool flag = (f.x()>start.x()-size)&&(f.x()<end.x()+size)&&(f.y()>start.y()-size)&&(f.y()<end.y()+size);
    return flag&&Inside_Last_Shape(f,start,end,size);
    */

    return  ((abs(f.x()-s_min_x)<=dotsize) && (abs(f.y()-s_min_y)<=dotsize))||((abs(f.x()-s_min_x)<=dotsize)&&(abs(f.y()-s_max_y)<=dotsize))
            || ((abs(f.x()-s_max_x)<=dotsize) &&(abs(f.y())-s_min_y)<=dotsize) || ((abs(f.x()-s_max_x)<=dotsize)&&(abs(f.y()-s_max_y)<=dotsize));

}

void Shape_solve::paint_edge(Shape *current,QPainter *painter,bool polygon)
{
    s_max_x=0;       // set to initial value
    s_min_x=INT_MAX;
    s_max_y=0;
    s_min_y = INT_MAX;

    int size = 2;
    QPen pen;
    QColor p(175,255,255,255);
    pen.setWidth(size);
    pen.setStyle(Qt::DashLine);
    pen.setColor(p);
    painter->setPen(pen);

    int type = current->init_type;
    if(type != s_Line && type != s_Pen)     // 多边形完成
    {

        vector<QPoint>my_curve = current->my_curve;
        int curve_size = (type == s_Polygon)?(my_curve.size()-1):4;

        for(int i=  0;i < curve_size;i++)
        {
            if(my_curve[i].x() > s_max_x)
            {
                s_max_x = my_curve[i].x();
            }
            if(my_curve[i].x() < s_min_x)
            {
                s_min_x = my_curve[i].x();
            }
            if(my_curve[i].y() > s_max_y)
            {
                s_max_y = my_curve[i].y();
            }
            if(my_curve[i].y()<s_min_y)
            {
                s_min_y = my_curve[i].y();
            }
        }
    }
    else if(type == s_Line)
    {
        QPoint start = current->startpoint,end = current->endpoint;
        s_max_x = end.x();
        s_min_x = start.x();
        s_max_y = end.y();
        s_min_y = start.y();
        if(end.x() < start.x())
        {
            s_max_x = start.x();
            s_min_x = end.x();
        }
        if(end.y() < start.y())
        {
            s_max_y = start.y();
            s_min_y = end.y();
        }
    }
    else if(type == s_Pen)            // 画笔
    {
        for(int i = 0;i < current->my_pen.size();i++)
        {
            if(current->my_pen[i].x() > s_max_x)
            {
                s_max_x = current->my_pen[i].x();
            }
            if(current->my_pen[i].x() < s_min_x)
            {
                s_min_x = current->my_pen[i].x();
            }
            if(current->my_pen[i].y() > s_max_y)
            {
                s_max_y = current->my_pen[i].y();
            }
            if(current->my_pen[i].y()<s_min_y)
            {
                s_min_y = current->my_pen[i].y();
            }
        }
    }

    painter->drawRect(s_min_x-size,s_min_y-size,s_max_x-s_min_x+3*size,s_max_y-s_min_y+3*size);

    int r = 1;
    pen.setWidth(dotsize);
    painter->setPen(pen);
    QPointF pointf;
    pointf.setX(s_min_x);
    pointf.setY(s_min_y);
    painter->drawEllipse(pointf,r,r);
    pointf.setX(s_min_x);
    pointf.setY(s_max_y);
    painter->drawEllipse(pointf,r,r);
    pointf.setX(s_max_x);
    pointf.setY(s_min_y);
    painter->drawEllipse(pointf,r,r);
    pointf.setX(s_max_x);
    pointf.setY(s_max_y);
    painter->drawEllipse(pointf,r,r);
}








