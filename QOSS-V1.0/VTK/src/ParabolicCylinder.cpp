#include "..\include\ParabolicCylinder.h"

ParabolicCylinder::ParabolicCylinder(const GraphTrans & _graphTrans)
{
	type = PARABOLICCYLINDER;
	graphTrans = _graphTrans;
	data.resize(16);
	for (auto & x : data)
		x = 0;
	setParameter(0.02, 0.04, 0, 0.2);
	updateData();
}

ParabolicCylinder::ParabolicCylinder(const GraphTrans & _graphTrans, 
	const std::vector<double>& parameter)
{
	type = PARABOLICCYLINDER;
	graphTrans = _graphTrans;
	this->focus = -parameter[6]/4;
	this->yMax = parameter[13];
	this->zMin = parameter[14];
	this->zMax = parameter[15];

	setData(parameter);

	updateData();
}

void ParabolicCylinder::setParameter(double focus, double yMax, double zMin, double zMax)
{
	this->focus = focus;
	this->yMax = yMax;
	this->zMin = zMin;
	this->zMax = zMax;
	data[1] = 1;
	data[6] = -4 * focus;
	data[11] = yMax * yMax / focus;
	data[12] = -yMax;
	data[13] = yMax;
	data[14] = zMin;
	data[15] = zMax;
	updateData();
}

QTreeWidgetItem * ParabolicCylinder::getTree()
{
	QTreeWidgetItem * tree = new QTreeWidgetItem;
	tree->setText(0, "ParabolicCylinder");
	tree->setData(0, Qt::UserRole, QVariant(1));
	tree->setData(1, Qt::UserRole, QVariant(PARABOLICCYLINDER));
	QTreeWidgetItem * treeFocus = new QTreeWidgetItem;
	treeFocus->setText(0, QString("Focus: ")+ QString::number(focus));
	
	QTreeWidgetItem * treeYMax = new QTreeWidgetItem;
	treeYMax->setText(0, QString("YMax: ") + QString::number(yMax));

	QTreeWidgetItem * treeZ = new QTreeWidgetItem;
	treeZ->setText(0, QString("z: ") + QString::number(zMin) + QString(" ~ ")
		+ QString::number(zMax));

	tree->addChild(treeFocus);
	tree->addChild(treeYMax);
	tree->addChild(treeZ);
	tree->addChild(getTransformTree());
	return tree;
}
