#pragma once
#include <QMainWindow>
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
#include "ui_mainwindow.h"
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
#include <QAction>
#include <QActionGroup>
#include <QLineEdit>
#include <QTimer>
#include "mywidget.h"
#include "openglw.h"

namespace Ui {
class MainWindow;
}

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();


 signals:
    void post_shape(int type);
    void post_color(QColor color);
    void post_back();
    void post_clear();
    void post_color2(QColor color);
    void save_file();

public slots:

    void on_actionCircle_triggered();

    void on_actionRectangle_triggered();

    void on_actionLine_2_triggered();

    void on_actionColor_triggered();

    void on_actionback_triggered();

    void on_actionclear_triggered();

    void on_actionPen_triggered();

    void on_actionfilling_color_triggered();

    void on_actionsave_as_triggered();

    void on_actionCurve_triggered();

    void dot_line_triggered();

private slots:
     void on_action2D_2_triggered();

     void on_action3D_2_triggered();

     void on_actionPolygon_triggered();

private:

    Mywidget *mywidget;
    openglw *glwidget;
    QTimer clk;

    Ui::MainWindow*ui;
};


