#include "mywidget.h"


void cout_point(QPoint c)
{
    cout<<c.rx()<<" "<<c.ry()<<endl;
}
void cout_array(bool *p)
{
    cout<<p[0]<<" "<<p[1]<<" "<<p[2]<<" "<<p[3]<<endl;
}

void set_point(QPoint &f,const QPoint &a,double centerx,double centery,double angle)
{
    f.rx() = centerx+(a.x()-centerx)*cos(angle)-(a.y()-centery)*sin(angle);
    f.ry() = centery+(a.x()-centerx)*sin(angle)+(a.y()-centery)*cos(angle);
}

void Mywidget::set_flags(QPoint a,bool* p,Shape *rect_dot)
{
    int min_x = rect_dot->my_curve[0].x(),min_y=rect_dot->my_curve[0].y();
    int max_x = rect_dot->my_curve[2].x(),max_y = rect_dot->my_curve[2].y();
    if(a.y() > max_y)
    {
        p[0]=false;
        p[1]=true;
    }
    else if(a.y() < min_y)
    {
        p[0]=true;
        p[1] = false;
    }
    else
    {
        p[0]=p[1]=false;
    }
    if(a.x() < min_x)
    {
        p[2] = false;
        p[3] = true;
    }
    else if(a.x() > max_x)
    {
        p[2]=true;
        p[3]=false;
    }
    else
    {
        p[2]=p[3]=false;
    }
}
bool Mywidget::judge_safe(bool *start,bool *end)          // 判断是否是安全的
{
    return ((start[0]&&end[0])||(start[1]&&end[1])||(start[2]&&end[2])||(start[3]&&end[3]));
}
bool Mywidget::judge_inside(bool *start)
{
    return !(start[0]||start[1]||start[2]||start[3]);
}
void Mywidget::get_cut_point(vector<QPoint>&cut_point,QPoint a,QPoint b,const Shape *rect_dot)
{
    int min_x = rect_dot->my_curve[0].x(),min_y=rect_dot->my_curve[0].y();
    int max_x = rect_dot->my_curve[2].x(),max_y = rect_dot->my_curve[2].y();
    double result_x=0,result_y = 0;
    double k = (b.y()-a.y())/((double)b.x()-a.x());
    double m = a.y()-k*a.x();
    int p_minx = (a.x()>b.x())?b.x():a.x();
    int p_miny = (a.y()>b.y())?b.y():a.y();
    int p_max_x = (a.x() >b.x())?a.x():b.x();
    int p_max_y = (a.y()>b.y())?a.y():b.y();


    result_x = (min_y-m)/k;      // 0-1
    if(result_x >= min_x && result_x<=max_x && result_x<=p_max_x && result_x >=p_minx)
    {
        cut_point.push_back(QPoint(result_x,min_y));
    }
    result_x = (max_y-m)/k;         // 2-3
    if(result_x >= min_x && result_x <= max_x && result_x<=p_max_x && result_x >=p_minx)
    {
        cut_point.push_back(QPoint(result_x,max_y));
    }

    result_y = k*max_x+m;
    if(result_y >= min_y && result_y <= max_y && result_y>=p_miny && result_y<=p_max_y)
    {
        cut_point.push_back(QPoint(max_x,result_y));
    }
    result_y = k*min_x+m;
    if(result_y>=min_y && result_y<=max_y && result_y>=p_miny && result_y<=p_max_y)
    {
        cut_point.push_back(QPoint(min_x,result_y));
    }

}
void Mywidget::shape_cutting()
{
    if(rect_dot != nullptr)
    {
        for(vector<Shape *>::iterator it = store_shape.begin();it != store_shape.end();)
        {
            if((*it)->init_type == T_Line)       // 仅仅裁剪直线
            {
                bool start[4]={false},end[4]={false};
                QPoint a = (*it)->startpoint, b = (*it)->endpoint;
                set_flags(a,start,rect_dot);
                set_flags(b,end,rect_dot);
                if(!judge_safe(start,end))    // 需要进行裁剪
                {
                    vector<QPoint>cut_point;
                    get_cut_point(cut_point,a,b,rect_dot);
                    if(judge_inside(start) && judge_inside(end)) // 线段正好在框内
                    {
                            // 该线段不需要修改
                    }
                    else if(judge_inside(start))    // 一个端点在框内
                    {
                       (*it)->endpoint = cut_point[0];
                    }
                    else if(judge_inside(end))
                    {
                        (*it)->startpoint = cut_point[0];
                    }
                    else              // 端点都不在框内
                    {
                       (*it)->startpoint = cut_point[0];
                        (*it)->endpoint = cut_point[1];
                    }

                    it++;

                }
                else
                {
                    it = store_shape.erase(it);    // 不在框内部的直线删除
                }
            }
            else
            {
                it = store_shape.erase(it);      // 其他图形删除
            }
        }
        rect_dot = nullptr;       // 取消虚线框
        update();
    }
}


void Mywidget::set_degree(QString degree)
{
    degree_get = degree;
    angle=degree.QString::toDouble() / 180 * PI;
}
void Mywidget::set_spining()
{
    if(current_type == T_Ellipse && degree_get.toInt()%90 != 0)
    {
        QMessageBox::warning(this,"Warning","椭圆仅支持垂直旋转");
        return;
    }
    else if(current_type == T_Pen)
    {
        QMessageBox::warning(this,"Warning","画笔不支持旋转");
        return;
    }
    else if(current_type != T_Dot_Line)
    {
        shape_spining();
        update();
    }
}
void Mywidget::shape_spining()
{
    if(currentItem != nullptr)
    {
        if(currentItem->init_type == T_Line)
        {
        QPoint a = currentItem->startpoint,b = currentItem->endpoint;
        double centerx= (a.x()+b.x())/2,centery = (a.y()+b.y())/2;
        set_point(currentItem->startpoint,a,centerx,centery,angle);
        set_point(currentItem->endpoint,b,centerx,centery,angle);
        }
        else if(currentItem->init_type != T_Pen)       // 包括四边形，椭圆，曲线,多边形
        {
            QPoint temp;
            double centerx=0,centery = 0;
            int temp_size = currentItem->my_curve.size();
            for(int i= 0;i < temp_size;i++)
            {
                centerx += currentItem->my_curve[i].x();
                centery += currentItem->my_curve[i].y();
            }
            centerx = centerx/temp_size;
            centery = centery/temp_size;
            for(int i = 0;i < temp_size;i++)
            {
                temp = currentItem->my_curve[i];
                set_point(currentItem->my_curve[i],temp,centerx,centery,angle);
            }
        }

        update();
    }
}
