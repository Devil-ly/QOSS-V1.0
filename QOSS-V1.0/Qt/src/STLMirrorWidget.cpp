#include "Qt/include/STLMirrorWidget.h"
#include <vector>
#include <QFileDialog>
#include "../util/Definition.h"
using namespace userInterface;

STLMirrorWidget::STLMirrorWidget(QWidget *parent)
{
	setWindowTitle(tr("Create paraboloid"));

	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/Paraboloid_coor.png");

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	//dimGroupBox
	BrowseBtn = new QPushButton(tr("Browse..."));
	connect(BrowseBtn, SIGNAL(clicked()),
		this, SLOT(on_BrowseBtn()));

	planeLineEdit = new QLineEdit;
	planeLineEdit->setText(tr(""));
	planeLineEdit->setEnabled(false);

	QGridLayout * layout3 = new QGridLayout;
	layout3->addWidget(planeLineEdit, 0, 0);
	layout3->addWidget(BrowseBtn, 0, 1);

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

STLMirrorWidget::~STLMirrorWidget()
{

}

void STLMirrorWidget::setMirror(Mirror *mirror)
{
	stlMirror = dynamic_cast<STLMirror*>(mirror);

	GraphTransWidget::setMirror(mirror);
}

void STLMirrorWidget::on_BrowseBtn()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open the file"),
		"",
		tr("*.stl"));
	if (!filename.isEmpty())
	{
		planeLineEdit->setText(filename);
	}
	stlMirror->setNameFile(filename.toStdString());
}