#include "Qt/include/CalculationWidget.h"
#include <QMessageBox>
#include <QFileDialog>

CalculationWidget::CalculationWidget(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Calculation"));
	
	//paraGroupBox
	freLabel = new QLabel(tr("Frequency(GHz)"));
	freLineEdit = new QLineEdit;
	freLineEdit->setText(tr("10"));
	freLineEdit->setEnabled(false);

	calModLabel = new QLabel(tr("Calculation model"));   // 计算模式 解析解优先还是剖分优先
	calModCombobox = new QComboBox;
	calModCombobox->addItem(tr("Mesh priority"));
	calModCombobox->addItem(tr("Analysis priority"));
	calModCombobox->setCurrentIndex(1);

	MeshLabel = new QLabel(tr("Mesh size"));
	MeshCombobox = new QComboBox;
	MeshCombobox->addItem(tr("Fine"));
	MeshCombobox->addItem(tr("Standard"));
	MeshCombobox->addItem(tr("Coarse"));
	MeshCombobox->setCurrentIndex(1);

	QGridLayout * layoutpara = new QGridLayout;
	layoutpara->addWidget(freLabel, 0, 0);
	layoutpara->addWidget(freLineEdit, 0, 1);
	layoutpara->addWidget(calModLabel, 1, 0);
	layoutpara->addWidget(calModCombobox, 1, 1);
	layoutpara->addWidget(MeshLabel, 2, 0);
	layoutpara->addWidget(MeshCombobox, 2, 1);

	paraGroupBox = new QGroupBox;
	paraGroupBox->setTitle(tr("Parameters"));
	paraGroupBox->setLayout(layoutpara);

	//surfaceNumGroupBox
	surfaceLabel = new QLabel(tr("Num of Mirrors"));
	surfaceCombobox = new QComboBox;
	surfaceCombobox->addItem(tr("1"));
	surfaceCombobox->addItem(tr("2"));
	surfaceCombobox->addItem(tr("3"));
	surfaceCombobox->setCurrentIndex(1);

	QGridLayout * layoutSel = new QGridLayout;
	layoutSel->addWidget(surfaceLabel, 0, 0);
	layoutSel->addWidget(surfaceCombobox, 0, 1);
	
	surfaceNumGroupBox = new QGroupBox;
	surfaceNumGroupBox->setTitle(tr("Add surfaces to calculate"));
	surfaceNumGroupBox->setLayout(layoutSel);

	//resultGroupBox
	resultLabel = new QLabel(tr("Num of Surfaces"));
	resultCombobox = new QComboBox;
	resultCombobox->addItem(tr("Default"));
	resultCombobox->addItem(tr("Custom"));
	connect(resultCombobox, SIGNAL(activated(int)), this, SLOT(resultComboboxChange(int)));

	distanceLabel = new QLabel(tr("Propagation distance"));
	widthLabel = new QLabel(tr("Width"));
	depthLaebl = new QLabel(tr("Depth"));
	distanceLineEdit = new QLineEdit(tr("0.5"));
	widthLineEdit = new QLineEdit(tr("1"));
	depthLineEdit = new QLineEdit(tr("1"));
	distanceLineEdit->setEnabled(false);
	widthLineEdit->setEnabled(false);
	depthLineEdit->setEnabled(false);

	QGridLayout * layoutRect = new QGridLayout;
	layoutRect->addWidget(resultLabel, 0, 0);
	layoutRect->addWidget(resultCombobox, 0, 1);
	layoutRect->addWidget(distanceLabel, 1, 0);
	layoutRect->addWidget(distanceLineEdit, 1, 1);
	layoutRect->addWidget(widthLabel, 2, 0);
	layoutRect->addWidget(widthLineEdit, 2, 1);
	layoutRect->addWidget(depthLaebl, 3, 0);
	layoutRect->addWidget(depthLineEdit, 3, 1);

	saveLabel = new QLabel(tr("Output address"));
	saveLineEdit = new QLineEdit;
	saveBtn = new QPushButton(tr("Browse..."));
	connect(saveBtn, SIGNAL(clicked()), this, SLOT(buttonSave()));

	QGridLayout * layoutSave = new QGridLayout;
	layoutSave->addWidget(saveLabel, 0, 0);
	layoutSave->addWidget(saveLineEdit, 0, 1);
	layoutSave->addWidget(saveBtn, 0, 2);

	QVBoxLayout * resultLayout = new QVBoxLayout;
	resultLayout->addLayout(layoutRect);
	resultLayout->addLayout(layoutSave);

	resultGroupBox = new QGroupBox;
	resultGroupBox->setTitle(tr("Result"));
	resultGroupBox->setLayout(resultLayout);

	QVBoxLayout * selectTabLayout = new QVBoxLayout;
	selectTabLayout->addWidget(paraGroupBox);
	selectTabLayout->addWidget(surfaceNumGroupBox);
	selectTabLayout->addWidget(resultGroupBox);

	QWidget * selectWidget = new QWidget;
	selectWidget->setLayout(selectTabLayout);

	tabWidget = new QTabWidget;
	tabWidget->addTab(selectWidget, tr("Surfaces"));
	//tabWidget->addTab(monitorWidget, tr("Monitors"));

	okBtn = new QPushButton(tr("OK"));
	cancelBtn = new QPushButton(tr("Cancel"));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(buttonOk()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(buttonCancel()));
	QHBoxLayout * layoutbtn = new QHBoxLayout;
	layoutbtn->addSpacing(100);
	layoutbtn->addWidget(okBtn, 0);
	layoutbtn->addWidget(cancelBtn, 0);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbtn);

	//returnNum = -1;
}

CalculationWidget::~CalculationWidget()
{
	
}

void CalculationWidget::setFre(double temp)
{
	this->freLineEdit->setText(QString::number(temp / 1e9));
}

void CalculationWidget::buttonCancel()
{
	close();
}

void CalculationWidget::buttonSave()
{
	QString filename = QFileDialog::getExistingDirectory(this, 
		"Select the path to save output...", "./");
	if (!filename.isEmpty())
	{
		saveLineEdit->setText(filename);
	}
}

void CalculationWidget::buttonOk()
{
	accept();
}

void CalculationWidget::resultComboboxChange(int Num)
{
	if (Num == 0) 
	{
		distanceLineEdit->setEnabled(false);
		widthLineEdit->setEnabled(false);
		depthLineEdit->setEnabled(false);
	}
	else
	{
		distanceLineEdit->setEnabled(true);
		widthLineEdit->setEnabled(true);
		depthLineEdit->setEnabled(true);
	}
}

double CalculationWidget::getFre()
{
	return freLineEdit->text().toDouble() * 1e9;
}

double CalculationWidget::getDistance()
{
	return distanceLineEdit->text().toDouble();
}

int CalculationWidget::getMirrorNum()
{
	return surfaceCombobox->currentIndex();
}
