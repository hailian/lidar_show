#include "mainwindow.h"
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>

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

    auto m_polygonItem = new QGraphicsPolygonItem();   // 定义一个多边形图元
    QPolygonF polygon;
    polygon << QPointF(-100.0, -150.0) << QPointF(-120.0, 150.0)
            << QPointF(320.0, 160.0) << QPointF(220.0, -140.0);
    m_polygonItem->setPolygon(polygon);
    m_polygonItem->setPen(pen);
    m_polygonItem->setFlag(QGraphicsItem::ItemIsMovable);


    scene->addItem(m_polygonItem);



    auto mainView = new QGraphicsView(scene);
    //mainView->resize(800, 600);

    mainLayout->addWidget(mainView);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
}

