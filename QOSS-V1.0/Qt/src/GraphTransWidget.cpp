#include "Qt/include/GraphTransWidget.h"

#include <QMessageBox>

GraphTransWidget::GraphTransWidget(QWidget *parent, int wayButton)
	: QDialog(parent)
{
	//setWindowTitle(tr("Create paraboloid"));

	
	//createWidget();
	this->wayButton = wayButton;
}

GraphTransWidget::~GraphTransWidget()
{

}

void GraphTransWidget::addNameLabel(QGridLayout * layout)
{
	// label
	label = new QLabel(tr("Label"));
	nameLineEidt = new QLineEdit;
	nameLineEidt->setText(tr("Paraboloid"));  // 待修改

	layout->addWidget(label, 0, 0);
	layout->addWidget(nameLineEidt, 0, 1);

}

void GraphTransWidget::addDefGroupBox(QGroupBox * _defGroupBox, QString filename)
{
	// page1
	//defGroupBox
	QLabel * imgLlabel;
	imgLlabel = new QLabel;
	imgLlabel->setPixmap(QPixmap(filename));

	QGridLayout * layout1 = new QGridLayout;
	layout1->addWidget(imgLlabel);

	//defGroupBox = new QGroupBox;
	_defGroupBox->setLayout(layout1);
	_defGroupBox->setTitle(tr("Definition methods"));
}

void GraphTransWidget::addBaseGroupBox(QGroupBox * _baseGroupBox)
{
	//baseGroupBox
	Ulabel = new QLabel(tr("U"));
	Vlabel = new QLabel(tr("V"));
	Nlabel = new QLabel(tr("N"));

	ULineEidt = new QLineEdit;
	VLineEidt = new QLineEdit;
	NLineEidt = new QLineEdit;

	ULineEidt->setText(tr("0.0"));
	VLineEidt->setText(tr("0.0"));
	NLineEidt->setText(tr("0.0"));

	connect(ULineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(VLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(NLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));

	QGridLayout * layout2 = new QGridLayout;
	layout2->addWidget(Ulabel, 0, 0);
	layout2->addWidget(Vlabel, 1, 0);
	layout2->addWidget(Nlabel, 2, 0);
	layout2->addWidget(ULineEidt, 0, 1);
	layout2->addWidget(VLineEidt, 1, 1);
	layout2->addWidget(NLineEidt, 2, 1);

	_baseGroupBox->setTitle(tr("Definition methods"));
	_baseGroupBox->setLayout(layout2);
}

void GraphTransWidget::addRotateWidget(QWidget * RotateWidget, QString filename)
{
	// page2
	//imgGroupBox
	imgLlabel1 = new QLabel;
	imgLlabel1->setPixmap(QPixmap(filename));

	QGridLayout * layout5 = new QGridLayout;
	layout5->addWidget(imgLlabel1);

	imgGroupBox = new QGroupBox;
	imgGroupBox->setLayout(layout5);
	imgGroupBox->setTitle(tr("Definition methods"));

	//RotateGroupBox
	xlabel = new QLabel(tr("X"));
	ylabel = new QLabel(tr("Y"));
	zlabel = new QLabel(tr("Z"));

	xRotateLineEidt = new QLineEdit;
	yRotateLineEidt = new QLineEdit;
	zRotateLineEidt = new QLineEdit;
	connect(xRotateLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(yRotateLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));
	connect(zRotateLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));

	xRotateLineEidt->setText(tr("1.0"));
	yRotateLineEidt->setText(tr("0.0"));
	zRotateLineEidt->setText(tr("0.0"));

	QGridLayout * layout6 = new QGridLayout;
	layout6->addWidget(xlabel, 0, 0);
	layout6->addWidget(ylabel, 1, 0);
	layout6->addWidget(zlabel, 2, 0);
	layout6->addWidget(xRotateLineEidt, 0, 1);
	layout6->addWidget(yRotateLineEidt, 1, 1);
	layout6->addWidget(zRotateLineEidt, 2, 1);

	RotateGroupBox = new QGroupBox;
	RotateGroupBox->setTitle(tr("Axis of rotation"));
	RotateGroupBox->setLayout(layout6);

	//thetaGroupBox
	thetalabel = new QLabel(tr("degree"));
	thetaLineEidt = new QLineEdit;
	thetaLineEidt->setText(tr("0.0"));

	connect(thetaLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_GraphChange(QString)));

	QGridLayout * layout7 = new QGridLayout;
	layout7->addWidget(thetalabel, 0, 0);
	layout7->addWidget(thetaLineEidt, 0, 1);

	thetaGroupBox = new QGroupBox;
	thetaGroupBox->setTitle(tr("Rotation angle"));
	thetaGroupBox->setLayout(layout7);

	QVBoxLayout * tabLayout2;
	tabLayout2 = new QVBoxLayout;
	tabLayout2->addWidget(imgGroupBox);
	tabLayout2->addWidget(RotateGroupBox);
	tabLayout2->addWidget(thetaGroupBox);

	RotateWidget->setLayout(tabLayout2);
}

void GraphTransWidget::addBtn(QGridLayout * _layoutbt, int wayButton)
{
	this->wayButton = wayButton;
	if (wayButton == 1)
	{
		createbtn = new QPushButton(tr("Ok"));
		addbtn = new QPushButton(tr("Apply"));
		closebtn = new QPushButton(tr("Cancle"));

		connect(createbtn, SIGNAL(clicked()), this, SLOT(buttonOk()));
		connect(addbtn, SIGNAL(clicked()), this, SLOT(buttonApply()));
		connect(closebtn, SIGNAL(clicked()), this, SLOT(buttonClose()));
	}
	else
	{
		createbtn = new QPushButton(tr("Create"));
		addbtn = new QPushButton(tr("Add"));
		closebtn = new QPushButton(tr("Close"));

		connect(createbtn, SIGNAL(clicked()), this, SLOT(buttonCreate()));
		connect(addbtn, SIGNAL(clicked()), this, SLOT(buttonAdd()));
		connect(closebtn, SIGNAL(clicked()), this, SLOT(buttonClose()));
	}

	_layoutbt->addWidget(createbtn, 0, 0);
	_layoutbt->addWidget(addbtn, 0, 1);
	_layoutbt->addWidget(closebtn, 0, 2);
}

void GraphTransWidget::buttonApply()
{
	emit sendData(6);
	
}

void GraphTransWidget::buttonOk()
{
	emit sendData(3);
}

void GraphTransWidget::on_GraphChange(QString t)
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
	init();
	emit sendData(5);
}

GraphTrans GraphTransWidget::getGraphTransData(Surface *_modleData,
	Parameterization* para, bool &ok1) const
{
	bool ok;
	GraphTrans graphTransPara;
	string temp;
	double tran_x = ULineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = ULineEidt->text().toStdString();
		if (isLegitimate(temp, tran_x))
		{
			para->addParaSurface(temp, _modleData);
			_modleData->addVarible(0, temp);
		}
		else
		{
			ok1 = false;
			ULineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_modleData->deleteVarible(0);

	double tran_y = VLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = VLineEidt->text().toStdString();
		if (isLegitimate(temp, tran_y))
		{
			para->addParaSurface(temp, _modleData);
			_modleData->addVarible(1, temp);
		}
		else
		{
			ok1 = false;
			VLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}

	}
	else
		_modleData->deleteVarible(1);

	double tran_z = NLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = NLineEidt->text().toStdString();
		if (isLegitimate(temp, tran_z))
		{
			para->addParaSurface(temp, _modleData);
			_modleData->addVarible(2, temp);
		}
		else
		{
			ok1 = false;
			NLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_modleData->deleteVarible(2);

	double rotate_x = xRotateLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = xRotateLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_x))
		{
			para->addParaSurface(temp, _modleData);
			_modleData->addVarible(3, temp);
		}
		else
		{
			ok1 = false;
			xRotateLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_modleData->deleteVarible(3);

	double rotate_y = yRotateLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = yRotateLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_y))
		{
			para->addParaSurface(temp, _modleData);
			_modleData->addVarible(4, temp);
		}
		else
		{
			ok1 = false;
			yRotateLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_modleData->deleteVarible(4);

	double rotate_z = zRotateLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = zRotateLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_z))
		{
			para->addParaSurface(temp, _modleData);
			_modleData->addVarible(5, temp);
		}
		else
		{
			ok1 = false;
			zRotateLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_modleData->deleteVarible(5);

	double rotate_theta = thetaLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = thetaLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_theta))
		{
			para->addParaSurface(temp, _modleData);
			_modleData->addVarible(6, temp);
		}
		else
		{
			ok1 = false;
			thetaLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}

	}
	else
		_modleData->deleteVarible(6);

	graphTransPara.setGraphTransPar(tran_x, tran_y,
		tran_z, rotate_x, rotate_y,rotate_z, rotate_theta);
	ok1 = true;
	return graphTransPara;	
}

GraphTrans GraphTransWidget::getGraphTransData(RaySource *
	_LightSource, bool &ok1) const
{
	bool ok;
	GraphTrans graphTransPara;
	string temp;
	double tran_x = ULineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = ULineEidt->text().toStdString();
		if (isLegitimate(temp, tran_x))
		{
			//para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(0, temp);
		}
		else
		{
			ok1 = false;
			ULineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_LightSource->deleteVarible(0);

	double tran_y = VLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = VLineEidt->text().toStdString();
		if (isLegitimate(temp, tran_y))
		{
			//para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(1, temp);
		}
		else
		{
			ok1 = false;
			VLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}

	}
	else
		_LightSource->deleteVarible(1);

	double tran_z = NLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = NLineEidt->text().toStdString();
		if (isLegitimate(temp, tran_z))
		{
			//para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(2, temp);
		}
		else
		{
			ok1 = false;
			NLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_LightSource->deleteVarible(2);

	double rotate_x = xRotateLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = xRotateLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_x))
		{
			//para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(3, temp);
		}
		else
		{
			ok1 = false;
			xRotateLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_LightSource->deleteVarible(3);

	double rotate_y = yRotateLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = yRotateLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_y))
		{
			//para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(4, temp);
		}
		else
		{
			ok1 = false;
			yRotateLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_LightSource->deleteVarible(4);

	double rotate_z = zRotateLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = zRotateLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_z))
		{
			//para->addParaSurface(temp, _modleData);
			_LightSource->addVarible(5, temp);
		}
		else
		{
			ok1 = false;
			zRotateLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_LightSource->deleteVarible(5);

	double rotate_theta = thetaLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = thetaLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_theta))
		{
			//para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(6, temp);
		}
		else
		{
			ok1 = false;
			thetaLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}

	}
	else
		_LightSource->deleteVarible(6);

	graphTransPara.setGraphTransPar(tran_x, tran_y,
		tran_z, rotate_x, rotate_y, rotate_z, rotate_theta);
	ok1 = true;
	return graphTransPara;
}

GraphTrans GraphTransWidget::getGraphTransData(Field * _LightSource,
	Parameterization* para, bool & ok1) const
{
	bool ok;
	GraphTrans graphTransPara;
	string temp;
	double tran_x = ULineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = ULineEidt->text().toStdString();
		if (isLegitimate(temp, tran_x))
		{
			para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(0, temp);
		}
		else
		{
			ok1 = false;
			ULineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_LightSource->deleteVarible(0);

	double tran_y = VLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = VLineEidt->text().toStdString();
		if (isLegitimate(temp, tran_y))
		{
			para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(1, temp);
		}
		else
		{
			ok1 = false;
			VLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}

	}
	else
		_LightSource->deleteVarible(1);

	double tran_z = NLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = NLineEidt->text().toStdString();
		if (isLegitimate(temp, tran_z))
		{
			para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(2, temp);
		}
		else
		{
			ok1 = false;
			NLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_LightSource->deleteVarible(2);

	double rotate_x = xRotateLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = xRotateLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_x))
		{
			para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(3, temp);
		}
		else
		{
			ok1 = false;
			xRotateLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_LightSource->deleteVarible(3);

	double rotate_y = yRotateLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = yRotateLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_y))
		{
			para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(4, temp);
		}
		else
		{
			ok1 = false;
			yRotateLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_LightSource->deleteVarible(4);

	double rotate_z = zRotateLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = zRotateLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_z))
		{
			para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(5, temp);
		}
		else
		{
			ok1 = false;
			zRotateLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}
	}
	else
		_LightSource->deleteVarible(5);

	double rotate_theta = thetaLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = thetaLineEidt->text().toStdString();
		if (isLegitimate(temp, rotate_theta))
		{
			para->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(6, temp);
		}
		else
		{
			ok1 = false;
			thetaLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return graphTransPara;
		}

	}
	else
		_LightSource->deleteVarible(6);

	graphTransPara.setGraphTransPar(tran_x, tran_y,
		tran_z, rotate_x, rotate_y, rotate_z, rotate_theta);
	ok1 = true;
	return graphTransPara;
}

void GraphTransWidget::setGraphTransData(const GraphTrans& graphTransPara)
{

	ULineEidt->setText(QString::number(graphTransPara.getTrans_x()));
	VLineEidt->setText(QString::number(graphTransPara.getTrans_y()));
	NLineEidt->setText(QString::number(graphTransPara.getTrans_z()));

	xRotateLineEidt->setText(QString::number(graphTransPara.getRotate_x()));
	yRotateLineEidt->setText(QString::number(graphTransPara.getRotate_y()));
	zRotateLineEidt->setText(QString::number(graphTransPara.getRotate_z()));

	thetaLineEidt->setText(QString::number(graphTransPara.getRotate_theta()));

}

QString GraphTransWidget::getGraphTransName() const
{
	return nameLineEidt->text();
}

void GraphTransWidget::setGraphTransName(QString name)
{
	nameLineEidt->setText(name);
}

void GraphTransWidget::setNameLineEdit(QString name)
{
	nameLineEidt->setText(name);
}

int GraphTransWidget::Num()
{
	return 0;
}

void GraphTransWidget::init()
{
	ULineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	VLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	NLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");

}

void GraphTransWidget::setAddBtn()
{
	addbtn->setText(tr("Apply"));
}

void GraphTransWidget::setNameHide(bool ok)
{
	if (ok)
	{
		label->hide();
		nameLineEidt->hide();
	}
	else
	{
		label->show();
		nameLineEidt->show();
	}
}

void GraphTransWidget::setConnect(bool ok)
{
	if (ok) 
	{
		connect(ULineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		connect(VLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		connect(NLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		connect(xRotateLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		connect(yRotateLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		connect(zRotateLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		connect(thetaLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
	}
	else
	{
		disconnect(ULineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		disconnect(VLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		disconnect(NLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		disconnect(xRotateLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		disconnect(yRotateLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		disconnect(zRotateLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
		disconnect(thetaLineEidt, SIGNAL(textChanged(QString)),
			this, SLOT(on_GraphChange(QString)));
	}

}

void GraphTransWidget::changeTransText(int index, string t)
{
	switch (index)
	{
	case 0:
		ULineEidt->setText(QString::fromStdString(t));
		break;
	case 1:
		VLineEidt->setText(QString::fromStdString(t));
		break;
	case 2:
		NLineEidt->setText(QString::fromStdString(t));
		break;
	case 3:
		xRotateLineEidt->setText(QString::fromStdString(t));
		break;
	case 4:
		yRotateLineEidt->setText(QString::fromStdString(t));
		break;
	case 5:
		zRotateLineEidt->setText(QString::fromStdString(t));
		break;
	case 6:
		thetaLineEidt->setText(QString::fromStdString(t));
		break;
	default:
		break;
	}
}

bool GraphTransWidget::isLegitimate(const string &express,
	double & val) const
{
	try
	{
		val = Parameterization::getVal(express);
	}
	catch (errorOperator &e)
	{
		int s = e.what();
		if (s == 0) //未能识别的操作符
		{
			QMessageBox::warning(NULL, "Warning",
				"Invalid data, Failed to identify the operator!");
			return false;
		}
		else if (s == 1) //无法计算
		{
			QMessageBox::warning(NULL, "Warning",
				"Invalid data, Failed to Calculate!");
			return false;
		}
		else if (s == 2) //未找到
		{
			QMessageBox::warning(NULL, "Warning",
				"Invalid data, Not find the corresponding parameters!");
			return false;
		}
		else
		{
			QMessageBox::warning(NULL, "Warning",
				"Invalid data, The input expression is incorrect!");
			return false;
		}
	}

	return true;
}

void GraphTransWidget::setWayButton(int t)
{
	wayButton = t;
}

void GraphTransWidget::closeEvent(QCloseEvent * event)
{
	if (wayButton == 1)
		emit sendData(4);
	else
		emit sendData(0);
}

void GraphTransWidget::buttonAdd()
{
	if (wayButton == 0)
		emit sendData(2);
	else
		emit sendData(6);
}

void GraphTransWidget::buttonCreate()
{
	if(wayButton == 0)
		emit sendData(1);
	else
		emit sendData(3);
	//close();
}

void GraphTransWidget::buttonClose()
{
	if(wayButton ==1)
		emit sendData(4);
	else
	    emit sendData(0);
	//returnNum = 0;
	//close();
}