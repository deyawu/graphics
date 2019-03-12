#include "mywidget.h"

void cout_color(QColor c)
{
    cout<<c.green()<<" "<<c.blue()<<" "<<c.red()<<" "<<endl;
}
 QColor Mywidget::get_from_point(int x,int y)
 {
     QColor c = myimg.pixel(x,y);
     return c;
 }
 void Mywidget::get_fill_item(int x,int y,QColor before_color,QColor new_color)
 {
     stack <QPoint> fill_stack;
     QPoint p(x,y);          // 种子点
     fill_stack.push(p);
     myimg.setPixelColor(p.x(),p.y(),new_color);
     while(!fill_stack.empty())
     {
         p = fill_stack.top(); // 从栈顶获取一个点
         fill_stack.pop();
         QPoint will_filled[4]; // 使用四连通，获取它周围的四个点
          will_filled[0] = QPoint(p.x()-1,p.y());
          will_filled[1]=QPoint(p.x()+1,p.y());
          will_filled[2]=QPoint(p.x(),p.y()+1);
          will_filled[3]=QPoint(p.x(),p.y()-1);
          for(int i = 0;i < 4;i++)
          {     // 只有当前的点在边界内部
              if( will_filled[i].x() > 0 && will_filled[i].y() > 0
                      && will_filled[i].x()<this->width() && will_filled[i].y()<this->height())
              {
                  QColor p_color=get_from_point(will_filled[i].x(),will_filled[i].y());
                  if( p_color == before_color)  // 需要填充的点是否符合要求
                  {
                      fill_stack.push(will_filled[i]);
                      myimg.setPixelColor(will_filled[i].x(),will_filled[i].y(),new_color);
                      // 对myimg进行像素上的操作
                  }
              }
          }
     }
 }
 void Mywidget::print_all_fillings(Shape *fill_shape)
 {
     int type = fill_shape->init_type;
     if(type != T_Rectangle && type != T_Ellipse && type != T_Polygon)
     {
         return;
     }
     if(fill_shape->enable_fill&& (! (fill_shape->shape_fill_color==Qt::white)))// 背景颜色
     {
     QPen pen;
     pen.setColor(fill_shape->shape_color);
     pen.setWidth(fill_shape->shape_size);

     int x = (fill_shape->my_curve[0].x()+fill_shape->my_curve[2].x())/2;        // 中心点的确定
     int y = (fill_shape->my_curve[0].y()+fill_shape->my_curve[2].y())/2;          // 中心点的确定
     if(type == T_Polygon)
     {

     }
     get_fill_item(x,y,Qt::white,fill_shape->shape_fill_color);  // use the background color
     }
 }
void Mywidget::paintEvent(QPaintEvent *event)
{
     myimg = QImage(this->width(),this->height(),QImage::Format_ARGB32);
     myimg.fill(Qt::white);
     handle_paintevent();

    QPainter painter(this);  // to be done;
    painter.drawImage(QRect(0,0,this->width(),this->height()),myimg);
}


void Mywidget::handle_paintevent()
{
    QPainter painter(&myimg);
    Shape *last_shape= nullptr;
    for(int i = 0;i < store_shape.size();i++)
    {
        QPen pen;
        pen.setColor(store_shape[i]->shape_color);
        pen.setWidth(store_shape[i]->shape_size);
        painter.setPen(pen);
        last_shape = store_shape[store_shape.size()-1];
        int type = store_shape[i]->init_type;
        if(type == T_Line)
        {
             store_shape[i]->my_draw_line(&painter);
        }
        else if(type == T_Curve)
        {
            store_shape[i]->my_draw_curve(&painter);
        }
        else if(type == T_Rectangle)
        {
            store_shape[i]->my_draw_rect(&painter);
        }
        else if(type == T_Ellipse)
        {
            store_shape[i]->my_draw_ellipse(&painter);
        }
        else if(type == T_Pen)
        {
            store_shape[i]->my_draw_pen(&painter);
        }
        else if(type == T_Polygon)
        {
            store_shape[i]->my_draw_polygon(&painter);
        }
        print_all_fillings(store_shape[i]);
    }
    if(pressed && currentItem != nullptr)
    {
        int type = currentItem->init_type;
        QPen pen;
        pen.setColor(currentItem->shape_color);
        pen.setWidth(currentItem->shape_size);
        painter.setPen(pen);
        if(type == T_Line)
        {
            currentItem->my_draw_line(&painter); // paint the current picture
        }
        else if(type == T_Curve)
        {
            currentItem->my_draw_curve(&painter);
        }
        else if(type== T_Ellipse)
        {
            currentItem->my_draw_ellipse(&painter);
        }
        else if(type == T_Rectangle)
        {
            currentItem->my_draw_rect(&painter);
        }
        else if(type == T_Pen)
        {
            currentItem->my_draw_pen(&painter);
        }
        else if(type == T_Polygon)
        {
            currentItem->my_draw_polygon(&painter);
        }
        print_all_fillings(currentItem);
    }
    if(rect_dot  != nullptr)
    {
        mysolve->paint_edge(rect_dot,&painter,is_polygon);
    }
    if(currentItem != nullptr && currentItem->init_type != T_Dot_Line)  //  不是虚线框
    {
       if((pressed && (resizing || draging)) || !pressed)
       {
           if(currentItem->init_type!=T_Curve ||  (currentItem->init_type==T_Curve && currentItem->my_curve.size() == 6))
           {
               if(currentItem->init_type != T_Polygon  ||
                       (currentItem->init_type==T_Polygon && currentItem->my_curve.size()>3)&& is_polygon)
               {
                   mysolve->paint_edge(currentItem,&painter,is_polygon);
               }
           }
       }

     }

}


void Mywidget::shape_clear()
{
    int len = store_shape.size();
    for(int i = 0;i < len;i++)
    {
       // cout<<store_shape[i]->enable_fill<<"  " <<store_shape[i]->init_type<<endl;
        store_shape.pop_back();
    }
    currentItem = nullptr;
    rect_dot = nullptr;
    is_polygon = true;

}
void Mywidget::shape_back()
{
    if(store_shape.size() != 0)
    {
        Shape *p = store_shape.back();
        if(p ->init_type == T_Polygon)
        {
            is_polygon = true;
        }
        store_shape.pop_back();
    }
    currentItem = nullptr;
}
void Mywidget::set_save_file()
{
    currentItem=nullptr;
    update();
    QFileDialog filelog;
    QString str = filelog.getSaveFileName(this,tr("open file"),"graphics.jpg",tr("Images(*.jpg)"));
    if(str == "")
    {
        QMessageBox::warning(this,tr("Warning"),tr("文件夹不能为空!"));
    }
    else
    {
        QPixmap pixmap = QPixmap::fromImage(myimg);
        pixmap.save(str);
    }

}
