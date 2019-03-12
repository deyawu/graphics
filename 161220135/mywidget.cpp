#include "mywidget.h"

Mywidget::Mywidget(QWidget *parent):QWidget (parent)
{
    pressed = false;
    saving_file=false;
    is_curve = false;
    is_polygon = true;
    color=Qt::black;
    size = 3;
    angle = 0;
    current_type=T_Rectangle;
    s_filling_color=Qt::black;
    filling = false;
    draging = false;
    resizing = false;
    mysolve = new Shape_solve();
    mouse_addr=QPoint(0,0);
    currentItem = nullptr;   // 初始化为空
    rect_dot = nullptr;       // 虚线框初始化为空

}

Mywidget::~Mywidget()
{

}

void Mywidget::mousePressEvent(QMouseEvent *m)
{
    setCursor(Qt::ArrowCursor);
    if(m->button() == Qt::LeftButton)
    {
        pressed = true;
        if((!store_shape.empty()||rect_dot!=nullptr) && currentItem != nullptr) // 图形变换的条件
        {
            mouse_addr = m->pos();
            if(rect_dot != nullptr)
            {
                currentItem = rect_dot;
            }
            else
            {
                currentItem = store_shape[store_shape.size()-1];
            }

            if(currentItem->init_type == T_Curve)            // 判断当前状态是否：曲线生成
            {
                is_curve = (currentItem->my_curve.size() == 6)?false:true;
            }
            if(is_curve)   // 生成曲线
            {
                currentItem->my_curve.push_back(m->pos());      // 加入鼠标点
                if(currentItem->my_curve.size()==5)
                {
                    currentItem->my_curve[1] = m->pos();
                    currentItem->my_curve[2]=m->pos();
                }
                else if(currentItem->my_curve.size()==6)
                {
                    currentItem->my_curve[2] = m->pos();
                }
            }
            else
            {
                if(currentItem->init_type == T_Polygon && is_polygon==false)
                {
                    return;                // 多边形未完成不能支持拖放
                }
                if(mysolve->Inside_Last_Shape(m->pos(),currentItem->startpoint,currentItem->endpoint,currentItem->shape_size))    // to do
                {
                    this->setCursor(Qt::SizeAllCursor);
                    draging = true;
                }
                else if(mysolve->On_Dot_Shape(m->pos(),currentItem->shape_size))
                {
                    resizing=true;
                }
                else          // ?????
                {
                    this->setCursor(Qt::CrossCursor);
                    addItem(m);
                }
            }
        }
        else
        {
            this->setCursor(Qt::CrossCursor);
            addItem(m);
        }
    }
    else if(m->button() == Qt::RightButton)
    {
        if(mysolve->Inside_Last_Shape(m->pos(),currentItem->startpoint,currentItem->endpoint,currentItem->shape_size))  // only fill the last shape
        {
            if(currentItem != nullptr && currentItem->init_type != T_Line && currentItem->init_type != T_Pen)
            {
                QMenu *contextmenu = new QMenu(this);
                QAction *my_fill_action = contextmenu->addAction("填充选择");
                QAction *my_not_fill_action=contextmenu->addAction("取消填充");
                connect(my_fill_action,SIGNAL(triggered()),this,SLOT(receive_fill_siganl()));
                connect(my_not_fill_action,SIGNAL(triggered()),this,SLOT(receive_not_fill_signal()));
                contextmenu->exec(QCursor::pos());               // need to put the statement after the connect
            }
        }
    }
    update();
}
void Mywidget::mouseMoveEvent(QMouseEvent *m)
{
    if(is_curve)
    {
        if(currentItem->my_curve.size()==5)
        {
            currentItem->my_curve[1] = m->pos();
            currentItem->my_curve[2] = m->pos();
        }
        else if(currentItem->my_curve.size()==6)
        {
            currentItem->my_curve[2] = m->pos();
        }
    }
    else
    {
        if(pressed && !draging &&!resizing)
        {
            currentItem->endpoint = m->pos();
            if(currentItem->init_type == T_Pen)
            {
                currentItem->my_pen.push_back(m->pos());
            }
            else if(currentItem->init_type == T_Curve)
            {
                currentItem->my_curve[3] = m->pos();
            }
            else if(currentItem->init_type ==T_Rectangle || currentItem->init_type == T_Ellipse || currentItem->init_type == T_Dot_Line)              // 移动过程中需要改变矩形边的顶点位置,矩形和椭圆，虚线框
            {
                currentItem->set_shape_point();
            }
        }
        else if(pressed && draging)
        {
            // cout<<"draging"<<endl;
            shape_draging(m,currentItem);
        }
        else if(pressed && resizing)
        {
            // cout<<"resizing"<<endl;
            shape_resizing(m,currentItem);
        }
    }
    update();
}
void Mywidget::mouseReleaseEvent(QMouseEvent *m)
{

    this->setCursor(Qt::ArrowCursor);
    if(pressed && !draging && !resizing &&!is_curve)
    {
        currentItem->endpoint=m->pos();
        currentItem->my_pen.push_back(m->pos());
        if(currentItem->init_type == T_Curve)                // 判断状态
        {
            currentItem->my_curve[3] = m->pos();
            currentItem->set_curve_point();
        }
        else if(currentItem->init_type == T_Rectangle || currentItem->init_type == T_Ellipse || currentItem->init_type == T_Dot_Line)
        {
            currentItem->set_shape_point();
        }
        else if(currentItem->init_type == T_Polygon)
        {
            if(is_polygon)        // 处于第一个点
            {
                currentItem->my_curve.push_back(currentItem->startpoint);
                is_polygon = false;
            }
            else
            {
                QPoint p = currentItem->my_curve[0];       // 取出起始点
                 if( (abs(p.x()-m->pos().x())<=10*currentItem->shape_size) && (abs(p.y()-m->pos().y())<=10*currentItem->shape_size))      // 给定区间
                {
                    if(currentItem->my_curve.size() >=3)
                    {
                        currentItem->my_curve.push_back(p);      // 结束多边形绘制
                        is_polygon = true;     // 恢复处于第一个点
                    }
                }
                else
                {
                    currentItem->my_curve.push_back(m->pos());
                }
            }
        }
    }
    update();
    pressed = false;
    draging = false;
    resizing = false;
    is_curve = false;

}
void Mywidget::shape_draging(QMouseEvent *m,Shape *current)
{
    if(current->init_type != T_Line && current->init_type != T_Pen)  // 椭圆，四边形，曲线，虚线框
    {
        int count_size = 0;
        if(current->init_type !=T_Curve)   count_size = current->my_curve.size();
        else  if(current->init_type == T_Curve)   count_size = current->my_curve.size()-2;
        for(int i=  0;i < count_size;i++)
        {
            current->my_curve[i].rx() += m->pos().x()-mouse_addr.x();
            current->my_curve[i].ry() += m->pos().y()-mouse_addr.y();
        }

    }
    else if(current->init_type ==T_Line)         // need to be done
    {
        current->startpoint.rx() += m->pos().x()-mouse_addr.x();
        current->startpoint.ry() += m->pos().y()-mouse_addr.y();
        current->endpoint.ry() +=  m->pos().y()-mouse_addr.y();
        current->endpoint.rx() += m->pos().x()-mouse_addr.x();
    }
    else
    {
        for(int i = 0;i < current->my_pen.size();i++)
        {
            current->my_pen[i].rx() +=  m->pos().x()-mouse_addr.x();
            current->my_pen[i].ry() += m->pos().y()-mouse_addr.y();
        }
    }
    mouse_addr = m->pos();    // 设置鼠标位置
}
void Mywidget::shape_resizing(QMouseEvent *m,Shape *current)     // 需要重写
{
    if(current->init_type == T_Pen)
    {
        QMessageBox::warning(this,tr("Warning"),tr("画笔不支持缩放"));
        this->setCursor(Qt::ArrowCursor);
        resizing = false;
    }
    else
    {
        int max_x,min_x,max_y,min_y;
        mysolve->getdot_xy(max_x,min_x,max_y,min_y);
        double centerx= (min_x+max_x)/2,centery = (min_y+max_y)/2;
        float mx = m->pos().x(),my=m->pos().y();
        double dietx=0,diety = 0;
        if(mx < centerx && my < centery)   // lefttop
        {
            dietx = (centerx-mx)/(centerx-min_x);
            diety = (centery-my)/(centery-min_y);
            this->setCursor(Qt::SizeFDiagCursor);
        }
        else if(mx < centerx && my > centery) // leftdown
        {
            dietx = (centerx-mx)/(centerx-min_x);
            diety = (my-centery)/(max_y-centery);
            this->setCursor(Qt::SizeBDiagCursor);
        }
        else if(mx > centerx && my < centery)  // rightup
        {
            dietx = (mx-centerx)/(max_x-centerx);
            diety = (centery-my)/(centery-min_y);
            this->setCursor(Qt::SizeBDiagCursor);
        }
        else if(mx > centerx && my > centery)  // rightdown
        {
            dietx = (mx-centerx)/(max_x-centerx);
            diety = (my-centery)/(max_y-centery);
            this->setCursor(Qt::SizeFDiagCursor);
        }
        if(fabs(mx-centerx) < 20)
        {
            dietx = 1;
        }
        if(fabs(my-centery)<20)
        {
            diety=1;
        }
        if(current->init_type ==T_Line)
        {
            current->startpoint.rx() = centerx + (current->startpoint.rx()-centerx)*dietx;
            current->startpoint.ry() = centery + (current->startpoint.ry()-centery)*diety;
            current->endpoint.rx() = centerx + (current->endpoint.rx()-centerx)*dietx;
            current->endpoint.ry() = centery + (current->endpoint.ry()-centery)*diety;
        }
        else                           // 有my_curve的曲线，四边形，椭圆，虚线框
        {
            int count_size = 0;
            if(current->init_type !=T_Curve)  count_size = current->my_curve.size();
            else  if(current->init_type == T_Curve)   count_size = current->my_curve.size()-2;
            for(int i=  0;i < count_size;i++)
            {
                QPoint p = current->my_curve[i];
                current->my_curve[i].rx() = (p.x()-centerx)*dietx + centerx;
                current->my_curve[i].ry() = (p.y()-centery)*diety + centery;
            }
        }
    }
    mouse_addr = m->pos();    // 设置鼠标位置；
}
void Mywidget::set_currentItem(QMouseEvent *m)
{
    Shape *s = new Shape();
    currentItem = s;
    currentItem->startpoint = m->pos();
    currentItem->endpoint = m->pos();
    currentItem->shape_color = color;
    currentItem->shape_size = size;
    currentItem->init_type = current_type;
    currentItem->enable_fill =false;
    currentItem->shape_fill_color = s_filling_color;
}
void Mywidget::addItem(QMouseEvent *m)
{
    if(current_type != T_Dot_Line)
    {
        rect_dot = nullptr;     //清除当前虚线框
        if(current_type == T_Pen || current_type == T_Line)
        {
            set_currentItem(m);
            store_shape.push_back(currentItem);
            currentItem->my_pen.push_back(m->pos());
        }
        else if(current_type == T_Rectangle || current_type==T_Ellipse || current_type == T_Curve)
        {
            set_currentItem(m);
            store_shape.push_back(currentItem);
            for(int i= 0;i < 4;i++)
            {
                currentItem->my_curve.push_back(m->pos());
            }
        }
        else if(current_type == T_Polygon)
        {
            if(is_polygon)
            {
            set_currentItem(m);
            store_shape.push_back(currentItem);
            }
        }
    }
    else if(current_type == T_Dot_Line)
    {
        rect_dot = new Shape();
        rect_dot ->init_type = T_Dot_Line;
        currentItem = rect_dot;
        currentItem->startpoint = m->pos();
        currentItem->startpoint = m->pos();

        rect_dot->my_curve.push_back(m->pos());
        rect_dot->my_curve.push_back(m->pos());
        rect_dot->my_curve.push_back(m->pos());
        rect_dot->my_curve.push_back(m->pos());
    }
}
