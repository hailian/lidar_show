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

    QPen pen3;
    pen3.setColor(QColor(255, 0, 0));
    pen3.setWidth(1);

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

    m_polygonItem3 = new QGraphicsPolygonItem();
    QPolygonF polygon3;
    polygon3 << QPointF(-100.0, -150.0)
            //<< QPointF(-120.0, 150.0)
            << QPointF(220.0, -140.0)
            << QPointF(320.0, 160.0) ;//
    m_polygonItem3->setPolygon(polygon3);
    m_polygonItem3->setPen(pen3);
    m_polygonItem3->setFlag(QGraphicsItem::ItemIsMovable);


    scene->addItem(m_polygonItem);
    scene->addItem(m_polygonItem2);
    scene->addItem(m_polygonItem3);

    auto mainView = new QGraphicsView(scene);
    //mainView->resize(800, 600);

    auto testBtn = new QPushButton;
    connect(testBtn,&QAbstractButton::pressed,[&](){
        QPointF pA(60.363108246330995,35.30580956282788);
        QPointF pB(50.910778473302884,-85.3258593488451);

        QPointF p1(263.9393092522702, 82.32398613379988);
        QPointF p2(264.2960017472896, 81.17155364978728);
        QPointF p3(262.5807537715334, 69.48941144874537);
        QPointF p4(117.93954061554737, 21.48207461609419);

        RightOfLine(p1,pA,pB);
        RightOfLine(p2,pA,pB);
        RightOfLine(p3,pA,pB);
        RightOfLine(p4,pA,pB);

//        QPolygonF polygon = m_polygonItem->polygon();
//        polygon.clear();

//        std::vector<QPointF> ps;
//        ps.push_back(QPointF(-100.0, -150.0));
//        ps.push_back(QPointF(-120.0, 150.0));
//        //ps.push_back(QPointF(220.0, -140.0));
//        ps.push_back(QPointF(320.0, 160.0));

//        this->removePoints(ps,0,ps.size()-1);

//        qDebug()<<ps.size();

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
    QPolygonF polygon2 = m_polygonItem2->polygon();
    QPolygonF polygon3 = m_polygonItem3->polygon();
    polygon.clear();
    polygon2.clear();
    polygon3.clear();
    const int bigSize = datas.size()/360;

    int pixMeter = 120;
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
            polygon <<QPointF(pos_x,pos_y);
        }
    }
    polygon <<QPointF(0,0);

    qDebug()<<"begin_size = "<<ps.size();

    //blue

    std::vector<QPointF> ps_red;
    //removePoints(ps,0,ps.size()-1,ps_red);

    for(int loop = 0;loop<3;loop++)
    {
        ps_red.clear();
        removePoints(ps,0,ps.size()/3,ps_red);
        removePoints(ps,ps.size()/3,ps.size()*2/3,ps_red);
        removePoints(ps,ps.size()*2/3,ps.size()-1,ps_red);
        if(ps_red.size()>7)
        {
            ps_red.erase(ps_red.begin());
            ps_red.erase(ps_red.begin());
            ps_red.erase(ps_red.begin());
            ps_red.erase(ps_red.begin());
        }
        ps = ps_red;
    }



    for(auto it = ps.begin();it!=ps.end();it++)
    {
        polygon2 <<*it;
    }
    polygon2 <<QPointF(0,0);
    qDebug()<<"ps_size = "<<ps.size();

    //ps_red.reserve(ps_red.size());
//    ps.reserve(ps.size());
//    ps_red.clear();
//    removePoints(ps,0,ps.size()-1,ps_red);

    for(auto it = ps_red.begin();it!=ps_red.end();it++)
    {
        polygon3 <<*it;
    }
    polygon3 <<QPointF(0,0);

    qDebug()<<"red_size = "<<ps_red.size();

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
    m_polygonItem3->setPolygon(polygon3);
}

void MainWindow::calcPoints(QPointF p1,QPointF p2,double &a,double &b)
{
    a = (p2.y() - p1.y() )/(p2.x() - p1.x());
    b = p2.y() - a * p2.x() ;
}

bool MainWindow::RightOfLine(const QPointF & pP, const QPointF& pA, const QPointF& pB)
{
    const double nSize = 1;
    QPointF PA((pA.x()-pP.x())/nSize,(pA.y()-pP.y())/nSize);
    QPointF PB((pB.x()-pP.x())/nSize,(pB.y()-pP.y())/nSize);

    double val = PA.x()*PB.y() - PA.y()*PB.x();

    if(val > 0)// >0 right
        return true;
    return false;
}

void MainWindow::removePoints(std::vector<QPointF> &ps,int begin,int end,std::vector<QPointF> &psRes)
{
    if(begin >=end){
        return;
    }

    int pos_head = begin;
    int pos_tail = end;

    QPointF p1 = ps[begin];
    QPointF p2 = ps[end];

    auto it = ps.begin() + pos_head;
    auto it_end = ps.begin() + pos_tail ;

    for(;it!=it_end;it++)
    {
        if(RightOfLine(*it,p1,p2))
        {
//            it = ps.erase(it);
//            continue;
        }else{
            psRes.push_back(*it);
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
