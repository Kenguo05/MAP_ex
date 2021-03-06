#pragma once

#include <QtWidgets/QMainWindow>
#include <QGraphicsRectItem>
#include "ui_MAP.h"

class QtWidgetsFL;
class QtWidgetsSE;
//////////////////////////////////////////////////////////////
class MAP : public QMainWindow
{
    Q_OBJECT
public:
    MAP(QWidget* parent = Q_NULLPTR);
    ~MAP();
private:
    Ui::MAPClass ui;
    QtWidgetsFL* fl = Q_NULLPTR;
    QtWidgetsSE* se = Q_NULLPTR;
    QTimer* m_Timer;
    QGraphicsItemGroup* gItem;
    void deleteItems();
protected:
    void closeEvent(QCloseEvent* event);
private  slots:
    void loadmap();
    void closewnd();
    void leasttran();
    void mindist();
    void findstop();
};
/////////////////////////////////////////////////////////////
class MyScene : public QGraphicsScene
{

public:
    explicit MyScene(QObject* parent = 0);
    void stopLines(QGraphicsView*);
    void clickmouse(int x, int y, int);
protected:
    QGraphicsView* qgv;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);//覆盖mousePressEvent以捕获鼠标事件
    void mousePressEvent(QGraphicsSceneMouseEvent* event);//覆盖mousePressEvent以捕获鼠标事件
signals:
public slots:
};
//////////////////////////////////////////////////////////////
class MyItem : public QGraphicsRectItem {
    int cx, cy;  //点击时的坐标
    int cf;      //左键点击=1，表示地点，右键点击=2表示终点
    int cs;      //靠近该点的坐标个数
    int bs[6];   //最多存放6个站点的站点编号
public:
    MyItem(int x, int y, int f);
    MyItem& operator<<(int s);
    int operator()(int x, int y);
    int& x();
    int& y();
    int& f();
    int& c();
    int& operator[](int);
    int checkAllStops();
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};
