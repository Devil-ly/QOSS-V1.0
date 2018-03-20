#include "..\include\Ellipsoid.h"

Ellipsoid::Ellipsoid(const GraphTrans & _graphTrans)
{
	type = ELLIPSOID;
	graphTrans = _graphTrans;
	data.resize(16);
	for (auto & x : data)
		x = 0;
	setParameter(0.1811, 0.1633, 0.1633, 180);
	updateData();
}

Ellipsoid::Ellipsoid(const GraphTrans & _graphTrans, 
	const std::vector<double>& parameter)
{
	type = ELLIPSOID;
	graphTrans = _graphTrans;

	setData(parameter);

	this->a = sqrt(1 / parameter[0]);
	this->b = sqrt(1 / parameter[1]);
	this->c = sqrt(1 / parameter[2]);

	updateData();
}

void Ellipsoid::setParameter(double a, double b, double c, double theta)
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->theta = theta;
	data[0] = 1 / a / a;
	data[1] = 1 / b / b;
	data[2] = 1 / c / c;
	data[9] = -1;
	data[10] = -a;
	data[11] = a;
	data[12] = -b;
	data[13] = b;
	data[14] = -c;
	data[15] = 0;

	// for test
	/*
	data[10] = a*0.6;
	data[11] = a*0.9;
	data[12] = -b*0.35;
	data[13] = b*0.35;
	data[14] = -c;
	data[15] = 0;*/
}

QTreeWidgetItem * Ellipsoid::getTree()
{
	QTreeWidgetItem * tree = new QTreeWidgetItem;
	tree->setText(0, "Ellipsoid");
	tree->setData(0, Qt::UserRole, QVariant(1));
	tree->setData(1, Qt::UserRole, QVariant(ELLIPSOID));
	QTreeWidgetItem * treea = new QTreeWidgetItem;
	treea->setText(0, QString("a: ") + QString::number(a));

	QTreeWidgetItem * treeb = new QTreeWidgetItem;
	treeb->setText(0, QString("b: ") + QString::number(b));

	QTreeWidgetItem * treec = new QTreeWidgetItem;
	treec->setText(0, QString("c: ") + QString::number(c));

	tree->addChild(treea);
	tree->addChild(treeb);
	tree->addChild(treec);
	tree->addChild(getTransformTree());
	return tree;
}

void Ellipsoid::setA(double a)
{
	setParameter(a, b, c, 0.2);
	updateData();
}

void Ellipsoid::setB(double b)
{
	setParameter(a, b, c, 0.2);
	updateData();
}

void Ellipsoid::setC(double c)
{
	setParameter(a, b, c, 0.2);
	updateData();
}
