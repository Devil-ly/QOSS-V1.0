#include "Qt/include/EllipsoidWidget.h"
#include <vector>
#include "../util/Definition.h"
using namespace userInterface;

EllipsoidWidget::EllipsoidWidget(QWidget *parent)
{
	setWindowTitle(tr("Create ellipsoid"));

	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/Paraboloid_coor.png");

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	//dimGroupBox
	alabel = new QLabel(tr("a"));
	blabel = new QLabel(tr("b"));
	clabel = new QLabel(tr("c"));

	aLineEidt = new QLineEdit;
	bLineEidt = new QLineEdit;
	cLineEidt = new QLineEdit;

	aLineEidt->setText(tr("0.2"));
	bLineEidt->setText(tr("0.16"));
	cLineEidt->setText(tr("0.16"));

	QGridLayout * layout3 = new QGridLayout;
	layout3->addWidget(alabel, 0, 0);
	layout3->addWidget(aLineEidt, 0, 1);
	layout3->addWidget(blabel, 1, 0);
	layout3->addWidget(bLineEidt, 1, 1);
	layout3->addWidget(clabel, 2, 0);
	layout3->addWidget(cLineEidt, 2, 1);

	dimGroupBox = new QGroupBox;
	dimGroupBox->setTitle(tr("Dimensions"));
	dimGroupBox->setLayout(layout3);

	//tabLayout1
	QVBoxLayout * tabLayout1; // page1
	tabLayout1 = new QVBoxLayout;
	tabLayout1->addWidget(defGroupBox);
	tabLayout1->addWidget(baseGroupBox);
	tabLayout1->addWidget(dimGroupBox);

	QWidget * widget1 = new QWidget;
	widget1->setLayout(tabLayout1);

	// page2
	//imgGroupBox
	QWidget * widget2;
	widget2 = new QWidget;
	addRotateWidget(widget2, "Paraboloid_coor.png");

	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));
	QGridLayout * layoutbt = new QGridLayout;

	addBtn(layoutbt);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbt);

}

EllipsoidWidget::~EllipsoidWidget()
{

}

void EllipsoidWidget::setMirror(Mirror *mirror)
{
	ellipsoid = dynamic_cast<Ellipsoid*>(mirror);

	GraphTransWidget::setMirror(mirror);
	aLineEidt->setText(QString::number(ellipsoid->getA()));
	bLineEidt->setText(QString::number(ellipsoid->getB()));
	cLineEidt->setText(QString::number(ellipsoid->getC()));

	connect(aLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_aChange(QString)));
	connect(bLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_bChange(QString)));
	connect(cLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_cChange(QString)));
}

void EllipsoidWidget::on_cChange(QString var)
{
	bool ok = false;
	double res = var.toDouble(&ok);
	if (!ok)
	{
		//输出参数有误
		cLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;

	}
	if (res <= 0.0 + THRESHOLD)
	{
		//输出参数有误 需要大于0
		cLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;
	}
	ellipsoid->setC(res);
	cLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}

void EllipsoidWidget::on_bChange(QString var)
{
	bool ok = false;
	double res = var.toDouble(&ok);
	if (!ok)
	{
		//输出参数有误
		bLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;

	}
	if (res <= 0.0 + THRESHOLD)
	{
		//输出参数有误 需要大于0
		bLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;
	}
	ellipsoid->setB(res);
	bLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}

void EllipsoidWidget::on_aChange(QString var)
{
	bool ok = false;
	double res = var.toDouble(&ok);
	if (!ok)
	{
		//输出参数有误
		aLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;
	}
	if (res <= 0.0 + THRESHOLD)
	{
		//输出参数有误 需要大于0
		aLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;

	}
	ellipsoid->setA(res);
	aLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}

