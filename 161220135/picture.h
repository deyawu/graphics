
#pragma once
#include <QMainWindow>
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

void my_draw_point(int x,int y,QPainter *my_painter);
void my_draw_line(QPoint start,QPoint end,int size,QColor mycolor,QPainter *my_painter);
void my_draw_ellipse(QPoint start,QPoint end,int size,QColor mycolor,QPainter *my_painter);
void my_draw_rect(QPoint start,QPoint end,int size,QColor mycolor,QPainter *my_painter);


