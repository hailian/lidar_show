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
    setMinimumSize(800,600);

    auto mainWidget = new QWidget(this);

    auto mainLayout = new QVBoxLayout;



    auto scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::darkGray);

    QPen pen;   // 定义一个画笔，设置画笔颜色和宽度
    pen.setColor(QColor(0, 255, 0));
    pen.setWidth(3);

    QPen pen2;   // 定义一个画笔，设置画笔颜色和宽度
    pen2.setColor(QColor(0, 0, 255));
    pen2.setWidth(1);

    m_polygonItem = new QGraphicsPolygonItem();   // 定义一个多边形图元
    QPolygonF polygon;
    polygon << QPointF(-100.0, -150.0) << QPointF(-120.0, 150.0)
            << QPointF(320.0, 160.0) << QPointF(220.0, -140.0);
    m_polygonItem->setPolygon(polygon);
    m_polygonItem->setPen(pen);
    m_polygonItem->setFlag(QGraphicsItem::ItemIsMovable);

    m_polygonItem2 = new QGraphicsPolygonItem();
    QPolygonF polygon2;
    polygon2 << QPointF(-100.0, -150.0)
            //<< QPointF(-120.0, 150.0)
            << QPointF(220.0, -140.0)
            << QPointF(320.0, 160.0) ;//
    m_polygonItem2->setPolygon(polygon2);
    m_polygonItem2->setPen(pen2);
    m_polygonItem2->setFlag(QGraphicsItem::ItemIsMovable);


    scene->addItem(m_polygonItem);
    scene->addItem(m_polygonItem2);



    auto mainView = new QGraphicsView(scene);
    //mainView->resize(800, 600);

    auto testBtn = new QPushButton;
    connect(testBtn,&QAbstractButton::pressed,[&](){

        QPolygonF polygon = m_polygonItem->polygon();
        polygon.clear();

//        polygon << QPointF(-200.0, -150.0) << QPointF(-220.0, 150.0)
//                << QPointF(220.0, 160.0) << QPointF(120.0, -140.0);
//        m_polygonItem->setPolygon(polygon);

        std::vector<QPointF> ps;
        ps.push_back(QPointF(-100.0, -150.0));
        //ps.push_back(QPointF(-120.0, 150.0));
        ps.push_back(QPointF(220.0, -140.0));
        ps.push_back(QPointF(320.0, 160.0));

        this->removePoints(ps,0,ps.size()-1);

        qDebug()<<ps.size();

    });
    testBtn->setText("test");
    mainLayout->addWidget(testBtn);

    mainLayout->addWidget(mainView);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    receiver = new UdpReceiver;
    connect(receiver , &UdpReceiver::readyRead,this,&MainWindow::onRecv);
}

MainWindow::~MainWindow()
{
}

void MainWindow::showLidar(std::vector<float> datas)
{
    QPolygonF polygon = m_polygonItem->polygon();
    polygon.clear();
    const int bigSize = datas.size()/360;

    int pixMeter = 135;
    int nums = datas.size();
    double offset = - 3.1415926 / 2 - 0.01;
#if 1
    double delta = 2 * 3.1415926 / nums;
    const int beginNum = 240 * bigSize;
    const int endNum = 330 * bigSize;
    std::vector<QPointF> ps;
    for (int i= beginNum ;i< endNum;i++ )
    {
        double yaw = i * delta + offset;
        double pos_x = -cos(yaw) * datas[i] * pixMeter;
        double pos_y = sin(yaw) * datas[i] * pixMeter;

        if(datas[i] != 0){
            ps.push_back(QPointF(pos_x,pos_y));
        }

        polygon <<QPointF(pos_x,pos_y);
    }
    polygon <<QPointF(0,0);

    //blue
    qDebug()<<"begin "<< ps.size();
    //this->removePoints(ps,ps.size()/2,ps.size()-1);

    if(ps.size()<3){
        return;
    }

//    for(auto it = ps.begin();it!=ps.end();it++)
//    {
//        i++;
////        if(it->x() == 0 && it->y() == 0)
////        {
////            it = ps.erase(it);
////            continue;
////        }
//    }

    int pos_center = 0;
    int loop_size = 5;


    for(int i =0;i<loop_size;i++){
        pos_center = ps.size()/2;
        this->removePoints(ps,1,ps.size()/5);
        this->removePoints(ps,ps.size()/5+1,ps.size()/5 *2);
        this->removePoints(ps,ps.size()/5*2+1,ps.size()/5 *3);
        this->removePoints(ps,ps.size()/5*3+1,ps.size()/5 *4);
        this->removePoints(ps,ps.size()/5*4+1,ps.size() -2);
    }

//    while(ps.size()>12){
//        this->removePoints(ps,0,ps.size()/5);
//        this->removePoints(ps,ps.size()/5+1,ps.size()/5 *2);
//        this->removePoints(ps,ps.size()/5*2+1,ps.size()/5 *3);
//        this->removePoints(ps,ps.size()/5*3+1,ps.size()/5 *4);
//        this->removePoints(ps,ps.size()/5*4+1,ps.size() -1);
//    }

    this->removePoints(ps,2,ps.size() -2);

    qDebug()<<"end "<< ps.size();

    QPolygonF polygon2 = m_polygonItem2->polygon();
    polygon2.clear();
    //for(auto it = ps.begin()+ ps.size()/2;it!=ps.end();it++)
    for(auto it = ps.begin();it!=ps.end();it++)
    {
        polygon2 << *it;

    }
    polygon2 <<QPointF(0,0);
#else
    //pixMeter = 1;
    double delta = 60;

    int base_degree = 270;

    int tail_num = (base_degree - delta/2 )* bigSize;
    int head_num = (base_degree + delta/2 )* bigSize;

    //qDebug()<<tail_num  << ","<<head_num;
    double value_head = (datas[head_num] + datas[head_num+3] + datas[head_num-3])/3;
    double value_tail = (datas[tail_num] + datas[tail_num+3] + datas[tail_num-3])/3;

    //qDebug()<<value_head << ","<<value_tail ;

    double yaw_tail = tail_num /bigSize/180.0 * 3.1415926;
    double yaw_head = head_num /bigSize/180.0 * 3.1415926;

    double pos_head_x = -cos(yaw_head) * value_head * pixMeter;
    double pos_head_y = -sin(yaw_head) * value_head * pixMeter;

    double pos_tail_x = -cos(yaw_tail) * value_tail * pixMeter;
    double pos_tail_y = -sin(yaw_tail) * value_tail * pixMeter;

    const double a = (pos_head_y - pos_tail_y )/(pos_head_x - pos_tail_x);
    const double b = pos_tail_y - a * pos_tail_x;
    qDebug()<< a << ","<<b/pixMeter;

    polygon <<QPointF(pos_head_x,pos_head_y)<<QPointF(pos_tail_x,pos_tail_y) <<QPointF(0,0) ;
#endif

    m_polygonItem->setPolygon(polygon);
    m_polygonItem2->setPolygon(polygon2);
}

void MainWindow::calcPoints(QPointF p1,QPointF p2,double &a,double &b)
{
    a = (p2.y() - p1.y() )/(p2.x() - p1.x());
    b = p2.y() - a * p2.x() ;
}

void MainWindow::removePoints(std::vector<QPointF> &ps,int begin,int end)
{
    int pos_head = begin;
    int pos_tail = end;

    double a=0,b=0;
    calcPoints(ps[pos_head],ps[pos_tail],a,b);
    if(std::isnan(a)|| std::isnan(b)){
        return;
    }

    qDebug()<<"a = "<<a << ",b = " << b;

    auto it = ps.begin() + pos_head + 1;
    auto it_end = ps.begin() + pos_tail ;
    double tmp_y;
    for(;it!=it_end;it++)
    {
        tmp_y = a * it->x() + b;
        //a = a-0.17;
        if(a > 0){
            if(tmp_y > it->y())// upper
            {
                it = ps.erase(it);
                continue;
            }

        }else{
            if(tmp_y < it->y())// upper
            {
                it = ps.erase(it);
                continue;
            }
        }

    }
}

void MainWindow::onRecv(QByteArray qba)
{
    QStringList lst = QString(qba).split("-");
    //qDebug()<<QString(qba);
    lst.removeFirst();
    std::vector<float> datas;
    for(int i =0;i<lst.size();i++)
    {
        QString num = lst.at(i);
        if(num == "inf"){
            datas.push_back(0);
        }else{
            datas.push_back(num.toFloat());
        }
    }
    showLidar(datas);
}
