#include "Qt/include/ParaboloidWidget.h"
#include <vector>

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

	// label
	QGridLayout * layout4 = new QGridLayout;
	addNameLabel(layout4);


	//tabLayout1
	QVBoxLayout * tabLayout1; // page1
	tabLayout1 = new QVBoxLayout;
	tabLayout1->addWidget(defGroupBox);
	tabLayout1->addWidget(baseGroupBox);
	tabLayout1->addWidget(dimGroupBox);
	tabLayout1->addLayout(layout4);

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

void ParaboloidWidget::setWidgetData(Surface const * _modleData)
{
	setConnect(false);
	disconnect(radiusLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	disconnect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	vector<double> para = _modleData->getData();
	radiusLineEidt->setText(QString::number(para[7]));
	depthLineEidt->setText(QString::number(para[8]));
	setGraphTransData(_modleData->getGraphTrans());
	list<pair<int, string>>varible = _modleData->getVarible();
	for (auto x : varible)
		changeText(x.first, x.second);
	connect(radiusLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	connect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	setConnect(true);
}

bool ParaboloidWidget::getWidgetData(Surface * _modleData,
	Parameterization* parameterization) const
{
	bool ok, ok_back;
	vector<double> para(9);
	string temp;
	para[7] = radiusLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = radiusLineEidt->text().toStdString();
		if (isLegitimate(temp, para[7]))
		{
			parameterization->addParaSurface(temp, _modleData);
			_modleData->addVarible(7, temp);
		}
		else
		{
			radiusLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}

	}
	else
		_modleData->deleteVarible(7);

	para[8] = depthLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = depthLineEidt->text().toStdString();
		if (isLegitimate(temp, para[8]))
		{
			parameterization->addParaSurface(temp, _modleData);
			_modleData->addVarible(8, temp);
		}
		else
		{
			depthLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_modleData->deleteVarible(8);

	ok = true;
	GraphTrans graphTransPara = getGraphTransData(_modleData, 
		parameterization,ok_back);
	ok = ok && ok_back;
	_modleData->setData(para);
	_modleData->setGraphTrans(graphTransPara);

	//QString name = getGraphTransName();
	//_surface.setSurfaceData(para, graphTransPara, name);
	return ok;
}

QString ParaboloidWidget::getName() const
{
	return getGraphTransName();
}

void ParaboloidWidget::changeText(int index, const string& t)
{
	if (index < 7)
		changeTransText(index, t);
	else if(index == 7)
		radiusLineEidt->setText(QString::fromStdString(t));
	else if (index == 8)
		depthLineEidt->setText(QString::fromStdString(t));
}

void ParaboloidWidget::on_DefChange(QString t)
{
	bool ok = false;
	t.toFloat(&ok);
	string temp;
	if (!ok)
	{
		temp = t.toStdString();
		try
		{
			Parameterization::getVal(temp);
		}
		catch (errorOperator &e)
		{
			return;
		}

	}
	radiusLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	depthLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(5);
}


