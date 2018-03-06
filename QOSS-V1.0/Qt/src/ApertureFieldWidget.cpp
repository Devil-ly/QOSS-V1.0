#include "Qt/include/ApertureFieldWidget.h"
#include <QFileDialog>
#include "../util/Definition.h"

using namespace userInterface;
ApertureFieldWidget::ApertureFieldWidget(QWidget *parentn)
{
	setWindowTitle(tr("Create Aperture-Field"));

	// page1
	//defGroupBox
	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/rectangle_coor.png");

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	//dimGroupBox
	widthlabel = new QLabel(tr("Width(W)"));
	depthlabel = new QLabel(tr("Depth(D)"));

	widthLineEdit = new QLineEdit;
	depthLineEdit = new QLineEdit;

	widthLineEdit->setText(tr("1"));
	depthLineEdit->setText(tr("1"));

	connect(widthLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(on_widthChange(QString)));
	connect(depthLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(on_depthChange(QString)));

	QGridLayout * layout3= new QGridLayout;
	layout3->addWidget(widthlabel, 0, 0);
	layout3->addWidget(depthlabel, 1, 0);
	layout3->addWidget(widthLineEdit, 0, 1);
	layout3->addWidget(depthLineEdit, 1, 1);

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
	QWidget * widget2 = new QWidget;
	addRotateWidget(widget2, "Qt/images/RotateWidget.png");

	// page3
	imgGroupBox = new QGroupBox;
	addDefGroupBox(imgGroupBox, "Qt/images/ApertureField_def.png");

	scalelabel   = new QLabel(tr("Magnitude scale factor"));
	phaselabel   = new QLabel(tr("Phase of aperture (deg)"));
	datadeflabel = new QLabel(tr("Aperture data definition"));

	scaleLineEidt = new QLineEdit;
	phaseLineEidt = new QLineEdit;

	scaleLineEidt->setText(tr("1"));
	phaseLineEidt->setText(tr("0"));

	datadefComboBox = new QComboBox;
	datadefComboBox->addItem(tr("Amplitude & Phase"));
	datadefComboBox->addItem(tr("Real & Imaginary"));

	QGridLayout * layout5 = new QGridLayout;
	layout5->addWidget(scalelabel, 0, 0);
	layout5->addWidget(phaselabel, 1, 0);
	layout5->addWidget(datadeflabel, 2, 0);
	layout5->addWidget(scaleLineEidt, 0, 1);
	layout5->addWidget(phaseLineEidt, 1, 1);
	layout5->addWidget(datadefComboBox, 2, 1);

	//sourcegroup
	Efieldlabel = new QLabel(tr("E-field file"));
	StartNumColumnlabel = new QLabel(tr("Start reading from Column number"));
	StartNumlabel = new QLabel(tr("Start reading from line number"));

	EfieldLineEidt = new QLineEdit;
	StartNumColumnEidt = new QLineEdit;
	StartNumLineEidt = new QLineEdit;

	StartNumLineEidt->setText(tr("1"));
	StartNumColumnEidt->setText(tr("1"));

	Efieldbtn = new QPushButton(tr("Browse..."));
	connect(Efieldbtn, SIGNAL(clicked()), this, SLOT(EfileEeven()));


	EfieldLineEidt->setEnabled(true);
	Efieldbtn->setEnabled(true);

	QGridLayout * layout6 = new QGridLayout;
	layout6->addWidget(Efieldlabel, 0, 0);
	layout6->addWidget(EfieldLineEidt, 0, 1);
	layout6->addWidget(Efieldbtn, 0, 2);

	QGridLayout * layout7 = new QGridLayout;
	layout7->addWidget(StartNumlabel, 0, 0);
	layout7->addWidget(StartNumLineEidt, 0, 1);
	layout7->addWidget(StartNumColumnlabel, 1, 0);
	layout7->addWidget(StartNumColumnEidt,1, 1);

	QVBoxLayout * tabLayoutSource = new QVBoxLayout;
	tabLayoutSource->addLayout(layout6);
	tabLayoutSource->addLayout(layout7);

	sourceGroupBox = new QGroupBox;
	sourceGroupBox->setTitle(tr("Source"));
	sourceGroupBox->setLayout(tabLayoutSource);

	//destinationGroupBox;
	UNumlabel = new QLabel(tr("Number of points along U"));
	VNumlabel = new QLabel(tr("Number of points along V"));

	UNumLineEidt = new QLineEdit;
	VNumLineEidt = new QLineEdit;

	QGridLayout * layout8 = new QGridLayout;
	layout8->addWidget(UNumlabel, 0, 0);
	layout8->addWidget(VNumlabel, 1, 0);
	layout8->addWidget(UNumLineEidt, 0, 1);
	layout8->addWidget(VNumLineEidt, 1, 1);

	destinationGroupBox = new QGroupBox;
	destinationGroupBox->setTitle(tr("Source destination"));
	destinationGroupBox->setLayout(layout8);

	//tabLayout2
	QVBoxLayout * tabLayout2; // page1
	tabLayout2 = new QVBoxLayout;
	tabLayout2->addWidget(imgGroupBox);
	tabLayout2->addLayout(layout5);
	tabLayout2->addWidget(sourceGroupBox);
	tabLayout2->addWidget(destinationGroupBox);

	QWidget * widget3 = new QWidget;
	widget3->setLayout(tabLayout2);

	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));
	tabWidget->addTab(widget3, tr("Excitation"));

	QGridLayout * layoutbtn = new QGridLayout;
	addBtn(layoutbtn);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbtn);

}

ApertureFieldWidget::~ApertureFieldWidget()
{
	
}

bool ApertureFieldWidget::getField(Field *& ptr)
{
	bool ok, ok_back;
	vector<double> para(14);
	string temp;
	para[0] = planeMirror->getWidth();
	para[1] = planeMirror->getDepth();

	para[2] = scaleLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		scaleLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}

	para[3] = phaseLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		phaseLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}

	QString EfieldFile = EfieldLineEidt->text();
	if (EfieldFile.isEmpty())
	{
		EfieldLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}

	para[4] = StartNumLineEidt->text().toInt(&ok);
	if (!ok)
	{
		StartNumLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}

	para[5] = StartNumColumnEidt->text().toInt(&ok);
	if (!ok)
	{
		StartNumColumnEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}

	int N = UNumLineEidt->text().toInt(&ok);
	if (!ok)
	{
		UNumLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}

	int M = VNumLineEidt->text().toInt(&ok);
	if (!ok)
	{
		VNumLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}

	para[6] = datadefComboBox->currentIndex();

	ptr = new ApertureField(planeMirror->getGraphTrans(), para);
	ptr->setNM(N, M);
	ptr->setDs(para[0] / (N - 1));
	dynamic_cast<ApertureField*>(ptr)->setFileAddress(EfieldFile.toStdString());
	dynamic_cast<ApertureField*>(ptr)->readData();
	ptr->updateData();
	return true;
}

void ApertureFieldWidget::EfileEeven()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open the file"),
		"",
		tr("*.txt"));
	if (!filename.isEmpty())
	{
		EfieldLineEidt->setText(filename);
	}
}

void ApertureFieldWidget::setMirror(Mirror *ptr)
{
	planeMirror = dynamic_cast<PlaneMirror*>(ptr); ;
	GraphTransWidget::setMirror(ptr);

	connect(widthLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(on_widthChange(QString)));
	connect(depthLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(on_depthChange(QString)));

}

void ApertureFieldWidget::on_widthChange(QString var)
{
	bool ok = false;
	double res = var.toDouble(&ok);
	if (!ok)
	{
		//输出参数有误
		widthLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;

	}
	if (res <= 0.0 + THRESHOLD)
	{
		//输出参数有误 需要大于0
		widthLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;
	}
	planeMirror->setWidth(res);
	widthLineEdit->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}

void ApertureFieldWidget::on_depthChange(QString var)
{
	bool ok = false;
	double res = var.toDouble(&ok);
	if (!ok)
	{
		//输出参数有误
		depthLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;
	}
	if (res <= 0.0 + THRESHOLD)
	{
		//输出参数有误 需要大于0
		depthLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
		return;

	}
	planeMirror->setDepth(res);
	depthLineEdit->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(2);
}
