#include "Qt/include/ParaboloidWidget.h"
#include <vector>
using namespace userInterface;

ParaboloidWidget::ParaboloidWidget(QWidget *parent, int wayButton)
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

	connect(radiusLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	connect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));

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

	addBtn(layoutbt, wayButton);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbt);

}

ParaboloidWidget::~ParaboloidWidget()
{

}

