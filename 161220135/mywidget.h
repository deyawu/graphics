#ifndef MYWIDGET_H
#define MYWIDGET_H

#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QColor>
#include <QPaintEvent>
#include <QImage>
#include <QPixmap>
#include <QPen>
#include <QPainter>
#include <QPointF>
#include <vector>
#include <iostream>
#include <QApplication>
#include <string.h>
#include <string>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QSpinBox>
#include <QSlider>
#include <QComboBox>
#include <math.h>
#include <QPalette>
#include <QPixmap>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <QMenu>
#include <QContextMenuEvent>
#include <stack>
#include <QDebug>
#include "shape_solve.h"
using namespace std;

#define PI 3.14159
class Mywidget : public QWidget ,public Shape
{
     Q_OBJECT
public:
    explicit Mywidget(QWidget *parent=nullptr);
    ~Mywidget();

    QColor get_from_point(int x,int y);
    void handle_paintevent();
    void addItem(QMouseEvent *m);
    void set_currentItem(QMouseEvent *m);
    void shape_draging(QMouseEvent *m,Shape *current);
    void shape_resizing(QMouseEvent *m,Shape *current);
    void mousePressEvent(QMouseEvent *m);
    void mouseMoveEvent(QMouseEvent *m);
    void mouseReleaseEvent(QMouseEvent *m);
    void paintEvent(QPaintEvent *event);
    void shape_clear();
    void shape_back();
    void get_fill_item(int x,int y,QColor before_color,QColor new_color);
    void print_all_fillings(Shape *fill_shape);
    void shape_spining();
    void shape_cutting();
    void set_flags(QPoint a,bool* p,Shape *rect_dot);
    bool judge_safe(bool *start,bool *end);
    bool judge_inside(bool *start);
    void get_cut_point(vector<QPoint>&cut_point,QPoint a,QPoint b,const Shape *rect_dot);

public slots:
    void set_save_file();
    void receive_fill_siganl()
    {
        if(currentItem != nullptr)
        {
            currentItem->enable_fill = true;
            currentItem->shape_fill_color=s_filling_color;
        }
    }
    void receive_not_fill_signal()
    {
        if(currentItem != nullptr)
        {
            currentItem->enable_fill = false;
        }
    }

    void set_degree(QString degree);
    void set_spining();         // 旋转槽函数
    void set_cutting(){shape_cutting();}        // 裁剪槽函数
    void set_shape(int type)
    {
        current_type = type;
        if(type != T_Polygon)
        {
            if(is_polygon == false) current_type = T_Polygon;
        }
        else  is_polygon =true;
    }
    void set_color(QColor mycolor){color = mycolor;}
    void set_size(int tsize){ size=tsize;}
    void set_filling_color(QColor filling_color){s_filling_color=filling_color;}
    void set_clear(){shape_clear();update();}
    void set_back(){shape_back();update();}


private:
    int current_type,size;
    bool pressed,saving_file,is_curve;
    QColor color;
    QColor s_filling_color;
    double angle;
    Shape *currentItem;
    Shape *rect_dot;
    vector<Shape *>store_shape;
    bool filling,draging,resizing;
    Shape_solve *mysolve;
    QPoint mouse_addr;    // used in QAction;
    QString degree_get;
    QImage myimg;
    bool is_polygon;
};

#endif // MYWIDGET_H
