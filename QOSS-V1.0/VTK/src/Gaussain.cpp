#include "../include/Gaussain.h"
#include "../util/Constant_Var.h"
//#include <Tools/Parameterization.h>

Gaussain::Gaussain(const GraphTrans & _graphTrans,
	const std::vector<double> parameter)
{
	//Name = GAUSSIAN;
	//factor = 1;
	isSource = true;
	//factor = _unit;
	//data = nullptr;

	graphTrans = _graphTrans;
	setData(parameter);
	updateData();

}

Gaussain::Gaussain(const Gaussain & _Gaussain)
	:factor(_Gaussain.factor)
{
	data.resize(_Gaussain.data.size());
	for (int i = 0; i < data.size(); i++)
		data[i] = _Gaussain.data[i];
}


/*Gaussain & Gaussain::operator=(const Gaussain & right)
{
	if (this == &right)
		return *this;

	factor = right.factor;
	varible.clear();
	for (auto x : right.varible)
		varible.push_back(x);
	data.resize(right.data.size());
	for (int i = 0; i < data.size(); i++)
		data[i] = right.data[i];

	content = right.content;
	N_width = right.N_width;
	M_depth = right.M_depth;
	ds = right.ds;
	isSource = right.isSource;
	actor = right.actor;
	actor3D = right.actor3D;
	isPhs = right.isPhs;
	isLinear = right.isLinear;
	is3D = right.is3D;
	allocateMemory();

	for (int i = 0; i < N_width; i++)
		for (int j = 0; j < M_depth; j++)
		{
			Ex[i][j] = right.Ex[i][j];
			Ey[i][j] = right.Ey[i][j];
			if (!isSource)
			{
				Ez[i][j] = right.Ez[i][j];
				Hx[i][j] = right.Hx[i][j];
				Hy[i][j] = right.Hy[i][j];
				Hz[i][j] = right.Hz[i][j];
			}
		}
	return *this;
}*/

Gaussain::~Gaussain()
{

}

void Gaussain::setUnit(double _unit)
{
	factor = _unit;
}

void Gaussain::updateData()
{
	//updateVarible();

	N_width = int(data[0] / data[5]) + 1;
	M_depth = int(data[1] / data[5]) + 1;

	setDs(data[5]);
	CreateGaussianGaussain();
	if (is3D)
		calActor3D();
	else
		calActor();
}


QTreeWidgetItem * Gaussain::getTree()
{
	QTreeWidgetItem * tree = new QTreeWidgetItem;
	tree->setText(0, "Gaussain");

	QTreeWidgetItem * treeWidth = new QTreeWidgetItem;
	treeWidth->setText(0, QString("Width: ") + QString::number(data[0]));

	QTreeWidgetItem * treeDepth = new QTreeWidgetItem;
	treeDepth->setText(0, QString("Depth: ") + QString::number(data[1]));

	QTreeWidgetItem * treez0 = new QTreeWidgetItem;
	treez0->setText(0, QString("z0: ") + QString::number(data[2]));

	QTreeWidgetItem * treew0 = new QTreeWidgetItem;
	treew0->setText(0, QString("w0: ") + QString::number(data[3]));

	QTreeWidgetItem * treeFre = new QTreeWidgetItem;
	treeFre->setText(0, QString("fre: ") + QString::number(data[4]));

	QTreeWidgetItem * treeds = new QTreeWidgetItem;
	treeds->setText(0, QString("ds: ") + QString::number(data[5]));

	tree->addChild(treeWidth);
	tree->addChild(treeDepth);
	tree->addChild(treez0);
	tree->addChild(treew0);
	tree->addChild(treeFre);
	tree->addChild(treeds);

	tree->addChild(getTransformTree());

	return tree;
}

void Gaussain::CreateGaussianGaussain()
{
	//生成Ex_In, Ey_IN;
	//double z0_back = z0; // 目标距离
	//double w0_back = w0; // 束腰
	//double fre_back = fre; // 频率
	allocateMemory();
	double z0 = data[2];
	double w0 = data[3];
	double fre = data[4];
	double ds = data[5];

	for (int i = 0; i < N_width; i++)
	{
		for (int j = 0; j < M_depth; j++)
		{
			double tempX = (i - (N_width - 1) / 2) * ds * factor;
			double tempY = (j - (M_depth - 1) / 2) * ds * factor;
			double tempr1;
			tempr1 = pow((tempX * tempX + tempY * tempY), 0.5);
			double temper, tempei;
			CreateGaussian(tempr1, w0 * factor, fre * 1e9, z0 * factor, temper, tempei);
			Ex[i][j] = complex<double>(0, 0);
			Ey[i][j] = complex<double>(temper, tempei);
		}
	}
	
}

void Gaussain::CreateGaussian(double r, double w0, double fre,
	double z0, double &Real, double &Imag) const {
	double lamda = C_Speed / fre;
	double k = 2 * Pi * fre / C_Speed;
	double w, Rdl, theta, r2, temp;

	w = w0 * pow((1 + pow(((lamda * z0) / (Pi * w0 * w0)), 2)), 0.5);
	Rdl = z0 / (z0 * z0 + pow((Pi * w0 * w0 / lamda), 2));
	theta = atan((lamda * z0) / (Pi * w0 * w0));
	r2 = r * r;
	temp = pow((2 / w / w / Pi), 0.5) * exp(-r2 / w / w);

	Real = temp * cos(theta - k * z0 - Pi * r2 / lamda * Rdl);
	Imag = temp * sin(theta - k * z0 - Pi * r2 / lamda * Rdl);
}