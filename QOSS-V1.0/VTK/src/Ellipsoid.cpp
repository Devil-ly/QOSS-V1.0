#include "..\include\Ellipsoid.h"

Ellipsoid::Ellipsoid(const GraphTrans & _graphTrans)
{
	type = ELLIPSOID;
	graphTrans = _graphTrans;
	data.resize(16);
	for (auto & x : data)
		x = 0;
	setParameter(0.08, 0.04, 0.04, 0.2);
	updateData();
}

Ellipsoid::Ellipsoid(const GraphTrans & _graphTrans, 
	const std::vector<double>& parameter)
{
	type = ELLIPSOID;
	graphTrans = _graphTrans;
	this->a = parameter[0];
	this->b = parameter[1];
	this->c = parameter[2];
	this->theta = parameter[3];
	data.resize(16);
	for (auto & x : data)
		x = 0;
	data[0] = 1 / a / a;
	data[1] = 1 / b / b;
	data[2] = 1 / c / c;
	data[9] = -1;
	data[10] = -a;
	data[11] = a;
	data[12] = -b;
	data[13] = b;
	data[14] = -c;
	data[15] = -0.5*c;
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
	data[15] = -0.5*c;
}

QTreeWidgetItem * Ellipsoid::getTree()
{
	QTreeWidgetItem * tree = new QTreeWidgetItem;
	tree->setText(0, "Ellipsoid");
	tree->setData(0, Qt::UserRole, QVariant(ELLIPSOID));
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
