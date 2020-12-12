#include "mainwindow.h"
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QDebug>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(1920,1080);

    auto mainWidget = new QWidget(this);

    auto mainLayout = new QVBoxLayout;



    auto scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::darkGray);

    QPen pen;   // 定义一个画笔，设置画笔颜色和宽度
    pen.setColor(QColor(0, 255, 0));
    pen.setWidth(10);

    m_polygonItem = new QGraphicsPolygonItem();   // 定义一个多边形图元
    QPolygonF polygon;
    polygon << QPointF(-100.0, -150.0) << QPointF(-120.0, 150.0)
            << QPointF(320.0, 160.0) << QPointF(220.0, -140.0);
    m_polygonItem->setPolygon(polygon);
    m_polygonItem->setPen(pen);
    m_polygonItem->setFlag(QGraphicsItem::ItemIsMovable);


    scene->addItem(m_polygonItem);



    auto mainView = new QGraphicsView(scene);
    //mainView->resize(800, 600);

    auto testBtn = new QPushButton;
    connect(testBtn,&QAbstractButton::pressed,[&](){

        QPolygonF polygon = m_polygonItem->polygon();
        polygon.clear();

//        polygon << QPointF(-200.0, -150.0) << QPointF(-220.0, 150.0)
//                << QPointF(220.0, 160.0) << QPointF(120.0, -140.0);
//        m_polygonItem->setPolygon(polygon);

        std::vector<float> datas;
        datas.push_back(1);
        datas.push_back(1);
        datas.push_back(1);

        datas.push_back(1);
        datas.push_back(1);
        datas.push_back(1);

        this->showLidar(datas);

    });
    testBtn->setText("test");
    mainLayout->addWidget(testBtn);


    mainLayout->addWidget(mainView);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::showLidar(std::vector<float> datas)
{
    QPolygonF polygon = m_polygonItem->polygon();
    polygon.clear();

    int pixMeter = 100;
    int nums = datas.size();
    double delta = 2 * 3.1415926 / nums;
    for (int i=0;i<nums ;i++ )
    {
        double yaw = i * delta;
        double pos_x = cos(yaw) * datas[i] * pixMeter;
        double pos_y = sin(yaw) * datas[i] * pixMeter;

        polygon <<QPointF(pos_x,pos_y);
    }
    m_polygonItem->setPolygon(polygon);
}
