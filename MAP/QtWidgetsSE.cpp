#include <QtTest/QtTest>
#include <qlistview.h>
#include <qcompleter.h>
#include <QFileDialog>
#include <QMessageBox>
#include "QtWidgetsSE.h"
#include "logiclayer.h"
#include "MAP.h"
#include <iostream>
using namespace std;

QtWidgetsSE::QtWidgetsSE(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButtonSearch, SIGNAL(clicked()), this, SLOT(inputloc()), Qt::UniqueConnection);
	connect(ui.pushButtonQuit, SIGNAL(clicked()), this, SLOT(close()), Qt::UniqueConnection);
	connect(ui.comboBoxStart, SIGNAL(currentIndexChanged(QString)), this, SLOT(searchstart(QString)), Qt::UniqueConnection);
	connect(ui.comboBoxEnd, SIGNAL(currentIndexChanged(QString)), this, SLOT(searchend(QString)), Qt::UniqueConnection);
}
void QtWidgetsSE::myShow(QGraphicsView* p) {
	parnt = p;
	show();
}
void QtWidgetsSE::readloc() {
	string a;
	char d;
	int b, c;
	location tmp;
	fstream fstrm;
	fstrm.open("organization.txt");
	if (!fstrm.is_open())
	{
		cout << "file open fail" << endl;
	}
	while (!fstrm.eof())
	{
		fstrm >> a >> b >> d >> c;
		tmp.la = a;
		tmp.lx = b;
		tmp.ly = c;
		loc.push_back(tmp);
		list << QString::fromStdString(a);
	}
	fstrm.close();
	ui.comboBoxStart->addItems(list);
	ui.comboBoxEnd->addItems(list);
}
QtWidgetsSE::~QtWidgetsSE()
{
}
void QtWidgetsSE::inputloc() {
	string begin = ui.comboBoxStart->currentText().toStdString();
	string end = ui.comboBoxEnd->currentText().toStdString();
	for (auto i : loc)
	{
		if (begin == i.la)
		{
			((MyScene*)(parnt->scene()))->clickmouse(i.lx, i.ly, 1);
			break;
		}
	}
	for (auto i : loc)
	{
		if (end == i.la)
		{
			((MyScene*)(parnt->scene()))->clickmouse(i.lx, i.ly, 2);
		}
	}
	close();
}
void QtWidgetsSE::searchstart(QString) {
	ui.comboBoxStart->setEditable(true);
	QCompleter* pCompleter = new QCompleter(list, this);
	pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	ui.comboBoxStart->setCompleter(pCompleter);
}
void QtWidgetsSE::searchend(QString) {
	ui.comboBoxEnd->setEditable(true);
	QCompleter* pCompleter = new QCompleter(list, this);
	ui.comboBoxEnd->setCompleter(pCompleter);
}
