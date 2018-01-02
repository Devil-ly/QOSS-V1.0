#include "../include/GaussianWidget.h"
#include <QFileDialog>
#include "../util/Definition.h"
using namespace userInterface;
GaussianWidget::GaussianWidget(QWidget *parent)
{
	setWindowTitle(tr("Create Gaussian"));

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
	addDefGroupBox(imgGroupBox, "Qt/images/Gaussian_def.png");

	//sourcegroup
	z0label = new QLabel(tr("Target distance(z0)"));
	w0label = new QLabel(tr("Gaussian waist(w0)"));
	frelabel = new QLabel(tr("Frequency  (GHz)"));
	dslabel = new QLabel(tr("Sampling Precision"));

	z0LineEdit = new QLineEdit;
	w0LineEdit = new QLineEdit;
	freLineEdit = new QLineEdit;
	DsLineEdit = new QLineEdit;

	z0LineEdit->setText("0");
	w0LineEdit->setText("0.04");
	freLineEdit->setText("10");
	DsLineEdit->setText("0.005");


	QGridLayout * layoutSou = new QGridLayout;
	layoutSou->addWidget(z0label, 0, 0);
	layoutSou->addWidget(w0label, 1, 0);
	layoutSou->addWidget(frelabel, 2, 0);
	layoutSou->addWidget(dslabel, 3, 0);
	layoutSou->addWidget(z0LineEdit, 0, 1);
	layoutSou->addWidget(w0LineEdit, 1, 1);
	layoutSou->addWidget(freLineEdit, 2, 1);
	layoutSou->addWidget(DsLineEdit, 3, 1);

	sourceGroupBox = new QGroupBox;
	sourceGroupBox->setTitle(tr("Source"));
	sourceGroupBox->setLayout(layoutSou);

	//tabLayout2
	QVBoxLayout * tabLayout2; // page1
	tabLayout2 = new QVBoxLayout;
	tabLayout2->addWidget(imgGroupBox);
	tabLayout2->addWidget(sourceGroupBox);

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
	mainlayout->addLayout(layoutbtn);/**/

}

GaussianWidget::~GaussianWidget()
{
	
}

bool GaussianWidget::getField(Field *& ptr)
{
	bool ok, ok_back;
	vector<double> para(6);
	string temp;
	para[0] = planeMirror->getWidth();
	para[1] = planeMirror->getDepth();	
	para[2] = z0LineEdit->text().toDouble(&ok);
	if (!ok)
	{		
		z0LineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}
	para[3] = w0LineEdit->text().toDouble(&ok);
	if (!ok)
	{		
		w0LineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}
	para[4] = freLineEdit->text().toDouble(&ok);
	if (!ok)
	{
		
		freLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}
	para[5] = DsLineEdit->text().toDouble(&ok);
	if (!ok)
	{		
		DsLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
		return false;
	}
	ptr = new Gaussain(planeMirror->getGraphTrans(), para);
	return true;
}

void GaussianWidget::setMirror(Mirror * ptr)
{
	planeMirror = dynamic_cast<PlaneMirror*>(ptr); ;
	GraphTransWidget::setMirror(ptr);

	connect(widthLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(on_widthChange(QString)));
	connect(depthLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(on_depthChange(QString)));
}

void GaussianWidget::on_widthChange(QString var)
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

void GaussianWidget::on_depthChange(QString var)
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


/*
void GaussianWidget::setWidgetData(Field const * _Source)
{
	setConnect(false);
	vector<double> para = _Source->getData();

	widthLineEdit->setText(QString::number(para[7]));
	depthLineEdit->setText(QString::number(para[8]));
	GraphTrans test = _Source->getGraphTrans();
	setGraphTransData(test);

	z0LineEdit->setText(QString::number(para[9]));
	w0LineEdit->setText(QString::number(para[10]));
	freLineEdit->setText(QString::number(para[11]));
	DsLineEdit->setText(QString::number(para[12]));
	
	list<pair<int, string>>varible = _Source->getVarible();
	for (auto x : varible)
		changeText(x.first, x.second);

	setConnect(true);
}

bool GaussianWidget::getWidgetData(Field * _Source,
	Parameterization* parameterization) const
{
	bool ok, ok_back;
	vector<double> para(13);
	string temp;
	para[7] = widthLineEdit->text().toDouble(&ok);
	if (!ok)
	{
		temp = widthLineEdit->text().toStdString();
		if (isLegitimate(temp, para[7]))
		{
			parameterization->addParaSurface(temp, _Source);
			_Source->addVarible(7, temp);
		}
		else
		{
			widthLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}

	}
	else
		_Source->deleteVarible(7);

	para[8] = depthLineEdit->text().toDouble(&ok);
	if (!ok)
	{
		temp = depthLineEdit->text().toStdString();
		if (isLegitimate(temp, para[8]))
		{
			parameterization->addParaSurface(temp, _Source);
			_Source->addVarible(8, temp);
		}
		else
		{
			depthLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_Source->deleteVarible(8);

	para[9] = z0LineEdit->text().toDouble(&ok);
	if (!ok)
	{
		temp = z0LineEdit->text().toStdString();
		if (isLegitimate(temp, para[9]))
		{
			parameterization->addParaSurface(temp, _Source);
			_Source->addVarible(9, temp);
		}
		else
		{
			z0LineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_Source->deleteVarible(9);

	para[10] = w0LineEdit->text().toDouble(&ok);
	if (!ok)
	{
		temp = w0LineEdit->text().toStdString();
		if (isLegitimate(temp, para[10]))
		{
			parameterization->addParaSurface(temp, _Source);
			_Source->addVarible(10, temp);
		}
		else
		{
			w0LineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_Source->deleteVarible(10);

	para[11] = freLineEdit->text().toDouble(&ok);
	if (!ok)
	{
		temp = freLineEdit->text().toStdString();
		if (isLegitimate(temp, para[11]))
		{
			parameterization->addParaSurface(temp, _Source);
			_Source->addVarible(11, temp);
		}
		else
		{
			freLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_Source->deleteVarible(11);

	para[12] = DsLineEdit->text().toDouble(&ok);
	if (!ok)
	{
		temp = DsLineEdit->text().toStdString();
		if (isLegitimate(temp, para[12]))
		{
			parameterization->addParaSurface(temp, _Source);
			_Source->addVarible(12, temp);
		}
		else
		{
			DsLineEdit->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_Source->deleteVarible(12);

	GraphTrans graphTransPara = getGraphTransData(_Source, 
		parameterization, ok_back);

	ok = true;
	ok = ok && ok_back;

	_Source->setData(para);
	_Source->setGraphTrans(graphTransPara);
	return ok;
}


void GaussianWidget::changeText(int index, const string& t)
{
	if (index < 7)
		changeTransText(index, t);
	else if (index == 7)
		widthLineEdit->setText(QString::fromStdString(t));
	else if (index == 8)
		depthLineEdit->setText(QString::fromStdString(t));
	else if (index == 9)
		z0LineEdit->setText(QString::fromStdString(t));
	else if (index == 10)
		w0LineEdit->setText(QString::fromStdString(t));
	else if (index == 11)
		freLineEdit->setText(QString::fromStdString(t));
	else if (index == 12)
		DsLineEdit->setText(QString::fromStdString(t));
}
*/