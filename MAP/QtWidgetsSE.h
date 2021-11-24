#pragma once
#include <map>
#include <utility>
#include <fstream>
#include <qmainwindow.h>
#include <QString>
#include <QWidget>
#include "ui_QtWidgetsSE.h"
using namespace std;

struct location {
	string la;
	int lx;
	int ly;
};
class QtWidgetsSE : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsSE(QWidget *parent = Q_NULLPTR);
	~QtWidgetsSE();
	QGraphicsView* parnt;
	void myShow(QGraphicsView* p);
	void readloc();
private:
	Ui::QtWidgetsSE ui;
	vector<location> loc;
	QStringList list;
private slots:
	void inputloc();
	void searchstart(QString);
	void searchend(QString);
};
