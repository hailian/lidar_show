#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPolygonItem>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showLidar(std::vector<float> datas);
private:
    QGraphicsPolygonItem *m_polygonItem;
};
#endif // MAINWINDOW_H
