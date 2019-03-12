#ifndef SHAPE_H
#define SHAPE_H

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
#include <Qpainter>
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
using namespace std;

enum mydrawing{T_Line,T_Curve,T_Pen,T_Rectangle,T_Ellipse,T_Dot_Line,T_Polygon};

class Shape
{
public:
    Shape();
    void my_draw_point(int x,int y,QPainter *my_painter);
    void my_draw_pen(QPainter *my_painter);
    void my_draw_curve(QPainter *my_painter);
    void my_draw_line(QPainter *my_painter);
    void drawing_line(QPoint start,QPoint end,QPainter *my_painter);
    void my_draw_ellipse(QPainter *my_painter);
    void my_draw_rect(QPainter *my_painter);
    void my_draw_dot_line(QPainter *my_painter);
    void my_draw_polygon(QPainter *my_painter);
    void set_shape_type(int type){init_type =  type;}
    void set_shape_color(QColor color){shape_color = color;}
    void set_shape_size(int size){shape_size = size;}
    void set_curve_point();
    void set_shape_point();
    ~Shape();

public:
    QPoint startpoint,endpoint;
    QColor shape_color;
    int shape_size;
    int init_type;
    vector<QPoint>my_pen;
    QColor shape_fill_color;
    bool enable_fill;
    vector<QPoint>my_curve;
};

#endif // SHAPE_H
