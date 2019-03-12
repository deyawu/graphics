#ifndef SHAPE_SOLVE_H
#define SHAPE_SOLVE_H
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
#include "shape.h"
using namespace std;
enum drawing{s_Line,s_Curve,s_Pen,s_Rectangle,s_Ellipse,s_Dot_Line,s_Polygon};
class Shape_solve
{
public:
    Shape_solve();
    bool Inside_Last_Shape(QPoint f,QPoint start,QPoint end,int size);
    bool On_Dot_Shape(QPoint f,int size);
    void paint_edge(Shape *current,QPainter *painter,bool polygon);
    void getdot_xy(int &x_max,int &x_min,int &y_max,int &y_min)
    {
        x_max = s_max_x;
        x_min = s_min_x;
        y_max = s_max_y;
        y_min = s_min_y;
    }
    void output_message(string c);
    ~Shape_solve(){}


private:
    int s_max_x,s_max_y,s_min_x,s_min_y;
    int dotsize;
};

#endif // SHAPE_SOLVE_H
