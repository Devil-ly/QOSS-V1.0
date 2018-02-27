#include "Qt/include/ParaboloidWidget.h"
#include <vector>
#include "../util/Definition.h"
using namespace userInterface;

ParaboloidWidget::ParaboloidWidget(QWidget *parent)
{
	setWindowTitle(tr("Create paraboloid"));

	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/Paraboloid_coor.png");

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	//dimGroupBox
	radiuslabel = new QLabel(tr("Radius(R)"));
	depthlabel = new QLabel(tr("Focal depth(F)"));

	radiusLineEidt = new QLineEdit;
	depthLineEidt = new QLineEdit;

	

	radiusLineEidt->setText(tr("1.0"));
	depthLineEidt->setText(tr("0.5"));

	QGridLayout * layout3 = new QGridLayout;
	layout3->addWidget(radiuslabel, 0, 0);
	layout3->addWidget(depthlabel, 1, 0);
	layout3->addWidget(radiusLineEidt, 0, 1);
	layout3->addWidget(depthLineEidt, 1, 1);

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

ParaboloidWidget::~ParaboloidWidget()
{

}

void ParaboloidWidget::setMirror(Mirror *mirror)
{
	paraboloid = dynamic_cast<Paraboloid*>(mirror);

	GraphTransWidget::setMirror(mirror);
	depthLineEidt->setText(QString::number(paraboloid->getFocus()));		
	radiusLineEidt->setText(QString::number(paraboloid->getRadius()));

	connect(radiusLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_radiusChange(QString)));
	connect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_focusChange(QString)));
}

void ParaboloidWidget::on_radiusChange(QString var)
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
	paraboloid->setRadius(res);
	radiusLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}

void ParaboloidWidget::on_focusChange(QString var)
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
	paraboloid->setFocus(res);
	depthLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}

