#include "..\include\MirrorTypeWidget.h"
#include <QVBoxLayout>
using namespace userInterface;

MirrorTypeWidget::MirrorTypeWidget(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Select the type of mirror"));

	//page1
	planeBtn = new QPushButton(tr("Plane"));
	ellipsoidBtn = new QPushButton(tr("Ellipsoid"));
	paraboloidBtn = new QPushButton(tr("ParaboloidBtn"));
	parabolicCylinderBtn = new QPushButton(tr("ParabolicCylinder"));

	QVBoxLayout * baseLayout = new QVBoxLayout;
	baseLayout->addWidget(planeBtn);
	baseLayout->addWidget(ellipsoidBtn);
	baseLayout->addWidget(paraboloidBtn);
	baseLayout->addWidget(parabolicCylinderBtn);

	QWidget * baseWidget = new QWidget;
	baseWidget->setLayout(baseLayout);

	//page2
	QWidget * customWidget = new QWidget;
	//baseWidget->setLayout(customLayout);

	//page3
	QWidget * importWidget = new QWidget;

	tabWidget = new QTabWidget;
	tabWidget->addTab(baseWidget, tr("Base"));
	tabWidget->addTab(customWidget, tr("Custom"));
	tabWidget->addTab(importWidget, tr("Import"));

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
}


MirrorTypeWidget::~MirrorTypeWidget()
{
}
