#include "Qt/include/ParabolicCylinderWidget.h"
#include <vector>
#include "../util/Definition.h"
using namespace userInterface;

ParabolicCylinderWidget::ParabolicCylinderWidget(QWidget *parent)
{
	setWindowTitle(tr("Create paraboloidCylinder"));

	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/Paraboloid_coor.png");

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	//dimGroupBox
	radiuslabel = new QLabel(tr("yMax"));
	depthlabel = new QLabel(tr("Focal depth(F)"));
	zmaxlabel = new QLabel(tr("zMax"));
	zminlabel = new QLabel(tr("zMin"));

	radiusLineEidt = new QLineEdit;
	depthLineEidt = new QLineEdit;
	zmaxLineEidt = new QLineEdit;
	zminLineEidt = new QLineEdit;	

	radiusLineEidt->setText(tr("1.0"));
	depthLineEidt->setText(tr("0.5"));
	zmaxLineEidt->setText(tr("1"));
	zminLineEidt->setText(tr("0"));


	QGridLayout * layout3 = new QGridLayout;
	layout3->addWidget(radiuslabel, 0, 0);
	layout3->addWidget(depthlabel, 1, 0);
	layout3->addWidget(radiusLineEidt, 0, 1);
	layout3->addWidget(depthLineEidt, 1, 1);
	layout3->addWidget(zmaxlabel, 2, 0);
	layout3->addWidget(zmaxLineEidt, 2, 1);
	layout3->addWidget(zminlabel, 3, 0);
	layout3->addWidget(zminLineEidt, 3, 1);

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

ParabolicCylinderWidget::~ParabolicCylinderWidget()
{

}

void ParabolicCylinderWidget::setMirror(Mirror *mirror)
{
	parabolicCylinder = dynamic_cast<ParabolicCylinder*>(mirror);

	GraphTransWidget::setMirror(mirror);
	depthLineEidt->setText(QString::number(parabolicCylinder->getFocus()));
	radiusLineEidt->setText(QString::number(parabolicCylinder->getRadius()));
	zmaxLineEidt->setText(QString::number(parabolicCylinder->getZMax()));
	zminLineEidt->setText(QString::number(parabolicCylinder->getZMin()));

	connect(radiusLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_radiusChange(QString)));
	connect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_focusChange(QString)));
	connect(zmaxLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_zmaxChange(QString)));
	connect(zminLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_zminChange(QString)));
}

void ParabolicCylinderWidget::on_radiusChange(QString var)
{
	bool ok = false;
	double res = var.toDouble(&ok);
	if (!ok)
	{
		//输出参数有误
		radiusLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;

	}
	if (res <= 0.0 + THRESHOLD)
	{
		//输出参数有误 需要大于0
		radiusLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;
	}
	parabolicCylinder->setRadius(res);
	radiusLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}

void ParabolicCylinderWidget::on_focusChange(QString var)
{
	bool ok = false;
	double res = var.toDouble(&ok);
	if (!ok)
	{
		//输出参数有误
		depthLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;
	}
	if (res <= 0.0 + THRESHOLD)
	{
		//输出参数有误 需要大于0
		depthLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;

	}
	parabolicCylinder->setFocus(res);
	depthLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}

void ParabolicCylinderWidget::on_zmaxChange(QString var)
{
	bool ok = false;
	double res = var.toDouble(&ok);
	if (!ok)
	{
		//输出参数有误
		zmaxLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;

	}
	if (res <= 0.0 + THRESHOLD)
	{
		//输出参数有误 需要大于0
		zmaxLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;
	}
	parabolicCylinder->setZMax(res);
	zmaxLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}

void ParabolicCylinderWidget::on_zminChange(QString var)
{

	bool ok = false;
	double res = var.toDouble(&ok);
	if (!ok)
	{
		//输出参数有误
		zminLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;

	}
	if (res <= 0.0 + THRESHOLD)
	{
		//输出参数有误 需要大于0
		zminLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;
	}
	parabolicCylinder->setZMin(res);
	zminLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}

