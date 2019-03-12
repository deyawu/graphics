#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int draw_1=50,draw_2=50,draw_3=1000,draw_4=800;
    setGeometry(draw_1,draw_2,draw_3,draw_4);

    mywidget = new Mywidget(this);
    setCentralWidget(mywidget);

    connect(this,SIGNAL(post_shape(int)),mywidget,SLOT(set_shape(int)));
    connect(this,SIGNAL(post_color(QColor)),mywidget,SLOT(set_color(QColor)));
    connect(this,SIGNAL(post_back()),mywidget,SLOT(set_back()));
    connect(this,SIGNAL(post_clear()),mywidget,SLOT(set_clear()));
    connect(this,SIGNAL(post_color2(QColor)),mywidget,SLOT(set_filling_color(QColor)));  // 颜色会出现覆盖的情况
    connect(this,SIGNAL(save_file()),mywidget,SLOT(set_save_file()));

    QSpinBox *spinbox=new QSpinBox(this);
    spinbox->setMinimum(2);
    spinbox->setMaximum(12);
    spinbox->setSingleStep(1);
    spinbox->setFixedSize(QSize(50,30));
    spinbox->setValue(3);
    connect(spinbox,SIGNAL(valueChanged(int)),mywidget,SLOT(set_size(int)));
    ui->toolBar->addWidget(spinbox);        // 添加spinbox
    ui->toolBar->addSeparator();

    QAction *dot_line = new QAction(QIcon(":/image/picture/dot.png"),tr("Dot line"),this);
    ui->toolBar->addAction(dot_line);
    connect(dot_line,SIGNAL(triggered()),this,SLOT(dot_line_triggered()));

    QAction *my_cut = new QAction(QIcon(":/image/picture/cut.png"),tr("Cut"),this);
    ui->toolBar->addAction(my_cut);
    connect(my_cut,SIGNAL(triggered()),mywidget,SLOT(set_cutting()));
    ui->toolBar->addSeparator();


    QLineEdit *p_edit = new QLineEdit();
    p_edit->setFixedSize(QSize(50,30));
    p_edit->setText("0");
    ui->toolBar->addWidget(p_edit);
    ui->toolBar->addSeparator();
    connect(p_edit,SIGNAL(textChanged(QString)),mywidget,SLOT(set_degree(QString)));

    QAction *spining = new QAction(QIcon(":/image/picture/spin.ico"),tr("Spin"),this);
    ui->toolBar->addAction(spining);
    connect(spining,SIGNAL(triggered()),mywidget,SLOT(set_spining()));
    ui->toolBar->addSeparator();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionCircle_triggered()
{
    emit post_shape(T_Ellipse);
}

void MainWindow::on_actionRectangle_triggered()
{
    emit post_shape(T_Rectangle);
}

void MainWindow::on_actionLine_2_triggered()
{
    emit post_shape(T_Line);
}

void MainWindow::on_actionColor_triggered()
{
    QColorDialog color;
    QColor temp_color = color.getRgba();
    emit post_color(temp_color);
}

void MainWindow::on_actionback_triggered()
{
    post_back();
}

void MainWindow::on_actionclear_triggered()
{
    post_clear();
}

void MainWindow::on_actionPen_triggered()
{
    emit post_shape(T_Pen);
}

void MainWindow::on_actionfilling_color_triggered()
{
    QColorDialog color2;
    QColor fillcolor = color2.getRgba();
    emit post_color2(fillcolor);
}

void MainWindow::on_actionsave_as_triggered()
{
    save_file();
}

void MainWindow::on_actionCurve_triggered()
{
    emit post_shape(T_Curve);
}

void MainWindow::dot_line_triggered()
{
    emit post_shape(T_Dot_Line);
}

void MainWindow::on_actionPolygon_triggered()
{
    emit post_shape(T_Polygon);
}


void MainWindow::on_action2D_2_triggered()
{
    if(glwidget != nullptr)
    {
        delete glwidget;
    }

}

void MainWindow::on_action3D_2_triggered()
{
    QFileDialog filelog;
    QString str = filelog.getOpenFileName(this,tr("open file"),".",tr("file(*.off)"));
    if(str == "")
    {
        QMessageBox::warning(this,tr("Warning"),tr("文件不能为空!"));
    }
    else
    {
        glwidget = new openglw(str);
        glwidget->show();
        clk.start(20);
        connect(&clk,SIGNAL(timeout()),glwidget,SLOT(update_3d()));
    }
}

