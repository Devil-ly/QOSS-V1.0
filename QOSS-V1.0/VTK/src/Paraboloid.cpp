#include "..\include\Paraboloid.h"

Paraboloid::Paraboloid(const GraphTrans & _graphTrans)
{
	type = PARABOLOID;
	graphTrans = _graphTrans;
	data.resize(16);
	for (auto & x : data)
		x = 0;
	setParameter(0.2, 0.1);
	updateData();
}

Paraboloid::Paraboloid(const GraphTrans & _graphTrans, 
	const std::vector<double>& parameter)
{
	type = PARABOLOID;
	graphTrans = _graphTrans;
	this->focus = parameter[0];
	this->radius = parameter[1];
	data.resize(16);
	for (auto & x : data)
		x = 0;
	data[0] = 1.0;
	data[1] = 1.0;
	data[8] = -4.0 * focus;
	data[10] = -radius;
	data[11] = radius;
	data[12] = -radius;
	data[13] = radius;
	data[14] = 0.0;
	data[15] = radius / 4.0 / focus;
	updateData();
}

void Paraboloid::setParameter(double focus, double radius)
{
	this->focus = focus;
	this->radius = radius;
	data[0] = 1.0;
	data[1] = 1.0;
	data[8] = -4.0 * focus;
	data[10] = -radius;
	data[11] = radius;
	data[12] = -radius;
	data[13] = radius;
	data[14] = 0.0;
	data[15] = radius * radius / 4.0 / focus;
}

QTreeWidgetItem * Paraboloid::getTree()
{
	QTreeWidgetItem * tree = new QTreeWidgetItem;
	tree->setText(0, "PARABOLOID");
	tree->setData(0, Qt::UserRole, QVariant(PARABOLOID));

	QTreeWidgetItem * treeFocus = new QTreeWidgetItem;
	treeFocus->setText(0, QString("Focus: ") + QString::number(focus));

	QTreeWidgetItem * treeRadius = new QTreeWidgetItem;
	treeRadius->setText(0, QString("Radius: ") + QString::number(radius));

	tree->addChild(treeFocus);
	tree->addChild(treeRadius);

	tree->addChild(getTransformTree());

	return tree;
}
