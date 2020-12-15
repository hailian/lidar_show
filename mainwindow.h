#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPolygonItem>
#include "udpreceiver.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showLidar(std::vector<float> datas);
    void calcPoints(QPointF p1,QPointF p2,double &a,double &b);
    bool RightOfLine(const QPointF & p, const QPointF& p1, const QPointF& p2);
    void removePoints(std::vector<QPointF> &ps,int begin,int end,std::vector<QPointF> &positive_ps);

public slots:
    void onRecv(QByteArray qba);
private:
    QGraphicsPolygonItem *m_polygonItem;
    QGraphicsPolygonItem *m_polygonItem2;
    QGraphicsPolygonItem *m_polygonItem3;

    UdpReceiver *receiver;


};
#endif // MAINWINDOW_H
